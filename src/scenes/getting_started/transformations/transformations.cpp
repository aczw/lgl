#include "transformations.hpp"
#include "../../../shaderprogram.hpp"
#include "../../../util.hpp"

#include <stb_image.h>
#include <array>
#include <cstdlib>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <string_view>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace lgl::scenes::transformations {

int main() {
  std::optional<GLFWwindow*> window_opt = util::create_window(1600, 1200);

  if (!window_opt) {
    return EXIT_FAILURE;
  }

  GLFWwindow* window = window_opt.value();

  // Doesn't need to be called every frame unless we're not sure that something else may modify it
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  constexpr std::array vertices = {
      // Position         // Color          // UV
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // Top left
  };

  constexpr std::array indices = {0, 1, 3, 1, 2, 3};

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

  GLuint ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

  ShaderProgram shader_prog("./shader.vert.glsl", "./shader.frag.glsl");

  int stride = static_cast<int>(8 * sizeof(float));

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<GLvoid*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<GLvoid*>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // OpenGL expects first pixel to be on the bottom left
  stbi_set_flip_vertically_on_load(true);

  int width = 0;
  int height = 0;
  int num_channels = 0;

  unsigned char* data = stbi_load(util::resolve_texture("./container.jpg").string().c_str(), &width,
                                  &height, &num_channels, 0);

  if (!data) {
    std::cout << "Failed to load image" << std::endl;
    return EXIT_FAILURE;
  }

  GLuint tex_0_handle = 0;
  glGenTextures(1, &tex_0_handle);
  glBindTexture(GL_TEXTURE_2D, tex_0_handle);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  data = stbi_load(util::resolve_texture("./awesomeface.png").string().c_str(), &width, &height,
                   &num_channels, 0);

  if (!data) {
    std::cout << "Failed to load image" << std::endl;
    return EXIT_FAILURE;
  }

  GLuint tex_1_handle = 0;
  glGenTextures(1, &tex_1_handle);
  glBindTexture(GL_TEXTURE_2D, tex_1_handle);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  shader_prog.use();
  shader_prog.set_uniform("tex_0", 0);
  shader_prog.set_uniform("tex_1", 1);

  constexpr glm::mat4 ident = glm::identity<glm::mat4>();
  constexpr glm::mat4 trans = glm::translate(ident, glm::vec3(0.5f, -0.5f, 0.0f));
  GLint trans_loc = shader_prog.get_uniform_location("u_Trans");

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    glClear(GL_COLOR_BUFFER_BIT);

    shader_prog.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_0_handle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex_1_handle);

    glm::mat4 rot = glm::rotate(trans, static_cast<float>(glfwGetTime()), util::z_axis);
    shader_prog.set_uniform("u_Trans", rot);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

}

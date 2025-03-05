#include "intro_shaders.hpp"
#include "../../shaderprogram.hpp"
#include "../../util.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <optional>
#include <string_view>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace lgl::scenes::intro_shaders {

namespace {
  void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }
}

int main() {
  std::optional<GLFWwindow*> window_opt = util::create_window(800, 600);

  if (!window_opt) {
    return EXIT_FAILURE;
  }

  GLFWwindow* window = window_opt.value();

  // Doesn't need to be called every frame unless we're not sure that something else may modify it
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  std::array vertices{
      // Position         // Color
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom left
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom right
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f   // Top center
  };

  std::array indices{0, 1, 2};

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

  int stride = 6 * sizeof(float);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClear(GL_COLOR_BUFFER_BIT);

    double time = glfwGetTime();
    double value = (std::sin(time) + 1.0f) * 0.5f;

    shader_prog.use();
    GLint unifColLocation = shader_prog.get_uniform_location("u_Col");
    glUniform4f(unifColLocation, 0.0f, static_cast<GLfloat>(value), 0.0f, 1.0f);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

}

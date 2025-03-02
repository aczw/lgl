#include <array>
#include <cstdlib>
#include <iostream>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "hello_triangle.hpp"

namespace learnogl::scenes::hello_triangle {

const char* const vs_src =
    "#version 460 core\n"
    "layout (location = 0) in vec3 vs_Pos;\n"
    "void main() {\n"
    "  gl_Position = vec4(vs_Pos, 1.0);\n"
    "}";

const char* const fs_src =
    "#version 460 core\n"
    "out vec4 out_Col;\n"
    "void main() {\n"
    "  out_Col = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}";

namespace {
  void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

  void check_shader_compile_status(GLuint shader) {
    int success = -1;
    std::array<char, 512> info_log{};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(shader, 512, NULL, info_log.data());
      std::cout << "error compiling shader: " << info_log.data() << std::endl;
    }
  }

  void check_shader_prog_link_status(GLuint shader_prog) {
    int success = -1;
    std::array<char, 512> info_log{};

    glGetProgramiv(shader_prog, GL_LINK_STATUS, &success);

    if (!success) {
      glGetProgramInfoLog(shader_prog, 512, NULL, info_log.data());
      std::cout << "error linking shader program: " << info_log.data() << std::endl;
    }
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "logl", nullptr, nullptr);

  if (!window) {
    std::cout << "failed to create GLFW window" << std::endl;
    glfwTerminate();

    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to init GLAD" << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow* /* window */, int width, int height) { glViewport(0, 0, width, height); });

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  std::array vertices{
      -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
  };

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vs_src, NULL);
  glCompileShader(vs);
  check_shader_compile_status(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fs_src, NULL);
  glCompileShader(fs);
  check_shader_compile_status(fs);

  GLuint shader_prog = glCreateProgram();
  glAttachShader(shader_prog, vs);
  glAttachShader(shader_prog, fs);
  glLinkProgram(shader_prog);
  check_shader_prog_link_status(shader_prog);

  glUseProgram(shader_prog);
  glDeleteShader(vs);
  glDeleteShader(fs);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_prog);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

}

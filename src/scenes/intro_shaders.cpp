#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "intro_shaders.hpp"

namespace lgl::scenes::intro_shaders {

const char* const vs_src =
    "#version 460 core\n"
    "layout (location = 0) in vec3 vs_Pos;\n"
    "layout (location = 1) in vec3 vs_Col;\n"
    "out vec4 fs_Col;\n"
    "void main() {\n"
    "  fs_Col = vec4(vs_Col, 1.0);"
    "  gl_Position = vec4(vs_Pos, 1.0);\n"
    "}";

const char* const fs_src =
    "#version 460 core\n"
    "uniform vec4 u_Col;\n"
    "in vec4 fs_Col;\n"
    "out vec4 out_Col;\n"
    "void main() {\n"
    "  out_Col = fs_Col;\n"
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
      glGetShaderInfoLog(shader, 512, nullptr, info_log.data());
      std::cout << "error compiling shader: " << info_log.data() << std::endl;
    }
  }

  void check_shader_prog_link_status(GLuint shader_prog) {
    int success = -1;
    std::array<char, 512> info_log{};

    glGetProgramiv(shader_prog, GL_LINK_STATUS, &success);

    if (!success) {
      glGetProgramInfoLog(shader_prog, 512, nullptr, info_log.data());
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

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to init GLAD" << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow* /* window */, int width, int height) { glViewport(0, 0, width, height); });

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

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vs_src, nullptr);
  glCompileShader(vs);
  check_shader_compile_status(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fs_src, nullptr);
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

    // Does not require us to first call glUseProgram (because we pass in the shader program we're querying?)
    GLint unifColLocation = glGetUniformLocation(shader_prog, "u_Col");

    glUseProgram(shader_prog);
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

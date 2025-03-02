#include <cstdlib>
#include <iostream>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ebo_rectangle.hpp"

namespace scenes::ebo_rectangle {

const char* vs_src =
    "#version 460 core\n"
    "layout (location = 0) in vec3 vs_Pos;\n"
    "void main() {\n"
    "  gl_Position = vec4(vs_Pos, 1.0);\n"
    "}";

const char* fs_src =
    "#version 460 core\n"
    "out vec4 out_Col;\n"
    "void main() {\n"
    "  out_Col = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}";

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void check_shader_compile_status(GLuint shader) {
  int success;
  char info_log[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, info_log);
    std::cout << "error compiling shader: " << info_log << std::endl;
  }
}

void check_shader_prog_link_status(GLuint shader_prog) {
  int success;
  char info_log[512];

  glGetProgramiv(shader_prog, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(shader_prog, 512, nullptr, info_log);
    std::cout << "error linking shader program: " << info_log << std::endl;
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
      window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  float vertices[] = {
      0.5f,  0.5f,  0.0f,  // Top right
      0.5f,  -0.5f, 0.0f,  // Bottom right
      -0.5f, -0.5f, 0.0f,  // Bottom left
      -0.5f, 0.5f,  0.0f   // Top left
  };

  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_prog);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

}  // namespace scenes::ebo_rectangle
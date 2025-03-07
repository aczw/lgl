#include "hello_triangle.hpp"

#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace lgl::scenes::hello_triangle {

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
      std::cout << "Error compiling shader: " << info_log.data() << std::endl;
    }
  }

  void check_shader_prog_link_status(GLuint shader_prog) {
    int success = -1;
    std::array<char, 512> info_log{};

    glGetProgramiv(shader_prog, GL_LINK_STATUS, &success);

    if (!success) {
      glGetProgramInfoLog(shader_prog, 512, NULL, info_log.data());
      std::cout << "Error linking shader program: " << info_log.data() << std::endl;
    }
  }
}

int main(Variant variant) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "lgl", nullptr, nullptr);

  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
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
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* /* window */, int width, int height) {
    glViewport(0, 0, width, height);
  });

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  std::vector<float> vertices{};
  std::vector<int> indices{};

  if (variant == Variant::triangle) {
    vertices = {
        -0.5f, -0.5f, 0.0f,  // Bottom left
        0.5f,  -0.5f, 0.0f,  // Bottom right
        0.0f,  0.5f,  0.0f,  // Center top
    };
    indices = {0, 1, 2};
  } else {
    vertices = {
        0.5f,  0.5f,  0.0f,  // Top right
        0.5f,  -0.5f, 0.0f,  // Bottom right
        -0.5f, -0.5f, 0.0f,  // Bottom left
        -0.5f, 0.5f,  0.0f   // Top left
    };
    indices = {0, 1, 3, 1, 2, 3};
  }

  int vertices_size = static_cast<int>(vertices.size() * sizeof(float));
  int indices_size = static_cast<int>(indices.size() * sizeof(int));

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices.data(), GL_STATIC_DRAW);

  GLuint ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices.data(), GL_STATIC_DRAW);

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
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
}

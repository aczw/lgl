#include "util.hpp"

#include <array>
#include <filesystem>
#include <format>
#include <iostream>
#include <optional>
#include <source_location>

namespace lgl::util {

namespace fs = std::filesystem;

std::optional<GLFWwindow*> create_window(int width, int height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(width, height, "lgl", nullptr, nullptr);

  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();

    return std::nullopt;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to init GLAD" << std::endl;
    glfwTerminate();

    return std::nullopt;
  }

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow* /* window */, int width, int height) {
    glViewport(0, 0, width, height);
  });

  return window;
}

bool check_shader_compile_status(GLuint shader, const std::source_location& src_loc) {
  int success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    std::array<char, 512> info_log{};
    glGetShaderInfoLog(shader, 512, nullptr, info_log.data());

    fs::path src_file = src_loc.file_name();
    std::cout << std::format("Error compiling shader in {}({}, {}):\n{}",
                             src_file.filename().string(), src_loc.line(), src_loc.column(),
                             info_log.data());
  }

  return success;
}

bool check_shader_program_link_status(GLuint shader_prog, const std::source_location& src_loc) {
  int success = 0;
  glGetProgramiv(shader_prog, GL_LINK_STATUS, &success);

  if (!success) {
    std::array<char, 512> info_log{};
    glGetProgramInfoLog(shader_prog, 512, nullptr, info_log.data());

    fs::path src_file = src_loc.file_name();
    std::cout << std::format("Error linking shader program in {}({}, {}):\n{}",
                             src_file.filename().string(), src_loc.line(), src_loc.column(),
                             info_log.data());
  }

  return success;
}

std::filesystem::path resolve_texture(std::string_view rel_path) {
  fs::path util_file = std::source_location::current().file_name();
  fs::path src_dir = util_file.parent_path();
  fs::path texture_dir = src_dir / "textures";

  return fs::canonical(texture_dir / rel_path);
}

}

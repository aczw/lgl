#include "shaderprogram.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <source_location>
#include <sstream>

namespace lgl {

namespace {
  void check_shader_compile_status(GLuint shader) {
    int success = -1;
    std::array<char, 512> info_log{};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(shader, 512, nullptr, info_log.data());
      std::cout << "lgl: Error compiling shader: " << info_log.data() << std::endl;
    }
  }

  void check_shader_prog_link_status(GLuint shader_prog) {
    int success = -1;
    std::array<char, 512> info_log{};

    glGetProgramiv(shader_prog, GL_LINK_STATUS, &success);

    if (!success) {
      glGetProgramInfoLog(shader_prog, 512, nullptr, info_log.data());
      std::cout << "lgl: Error linking shader program: " << info_log.data() << std::endl;
    }
  }
}

ShaderProgram::ShaderProgram(std::string_view rel_vs_path, std::string_view rel_fs_path, std::source_location src_loc) {
  std::filesystem::path src_file = src_loc.file_name();
  std::filesystem::path base_dir = src_file.parent_path();

  std::ifstream vs_file(base_dir / rel_vs_path);
  std::ifstream fs_file(base_dir / rel_fs_path);

  if (!vs_file.is_open() || !fs_file.is_open()) {
    std::cout << "ShaderProgram::ShaderProgram(): unable to open vertex or fragment shader file" << std::endl;
    return;
  }

  std::stringstream vs_buffer;
  std::stringstream fs_buffer;
  vs_buffer << vs_file.rdbuf();
  fs_buffer << fs_file.rdbuf();

  // Can't figure out how to directly pass it to glShaderSource()
  std::string vs = vs_buffer.str();
  const char* vs_c_str = vs.c_str();
  std::string fs = fs_buffer.str();
  const char* fs_c_str = fs.c_str();

  GLuint vs_handle = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs_handle, 1, &vs_c_str, nullptr);
  glCompileShader(vs_handle);
  check_shader_compile_status(vs_handle);

  GLuint fs_handle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs_handle, 1, &fs_c_str, nullptr);
  glCompileShader(fs_handle);
  check_shader_compile_status(fs_handle);

  handle = glCreateProgram();

  glAttachShader(handle, vs_handle);
  glAttachShader(handle, fs_handle);
  glLinkProgram(handle);
  check_shader_prog_link_status(handle);

  glDeleteShader(vs_handle);
  glDeleteShader(fs_handle);
}

void ShaderProgram::use() {
  glUseProgram(handle);
}

GLint ShaderProgram::get_uniform_location(const std::string& name) const {
  return glGetUniformLocation(handle, name.c_str());
}

void ShaderProgram::set_bool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(handle, name.c_str()), static_cast<int>(value));
}

void ShaderProgram::set_int(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(handle, name.c_str()), value);
}

void ShaderProgram::set_float(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(handle, name.c_str()), value);
}
}

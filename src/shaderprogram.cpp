#include "shaderprogram.hpp"
#include "util.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <source_location>
#include <sstream>
#include <string>
#include <string_view>

namespace lgl {

ShaderProgram::ShaderProgram(std::string_view rel_vs_path,
                             std::string_view rel_fs_path,
                             std::source_location src_loc) {
  std::filesystem::path src_file = src_loc.file_name();
  std::filesystem::path base_dir = src_file.parent_path();

  // Constructor automatically opens the files
  std::ifstream vs_file(base_dir / rel_vs_path);
  std::ifstream fs_file(base_dir / rel_fs_path);

  if (!vs_file.is_open() || !fs_file.is_open()) {
    std::cout << "ShaderProgram::ShaderProgram(): unable to open vertex or fragment shader file"
              << std::endl;
    return;
  }

  // Passing the file's stream buffer to the `<<` operator passes the entire contents of the file
  std::stringstream vs_stream;
  std::stringstream fs_stream;
  vs_stream << vs_file.rdbuf();
  fs_stream << fs_file.rdbuf();

  // Avoid potentially allocating a new string
  std::string_view vs = vs_stream.view();
  std::string_view fs = fs_stream.view();
  int vs_size = static_cast<int>(vs.size());
  int fs_size = static_cast<int>(fs.size());
  const char* vs_data = vs.data();
  const char* fs_data = fs.data();

  GLuint vs_handle = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs_handle, 1, &vs_data, &vs_size);
  glCompileShader(vs_handle);

  if (!util::check_shader_compile_status(vs_handle, src_loc)) {
    return;
  }

  GLuint fs_handle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs_handle, 1, &fs_data, &fs_size);
  glCompileShader(fs_handle);

  if (!util::check_shader_compile_status(fs_handle, src_loc)) {
    return;
  }

  handle = glCreateProgram();

  glAttachShader(handle, vs_handle);
  glAttachShader(handle, fs_handle);
  glLinkProgram(handle);

  if (!util::check_shader_program_link_status(handle, src_loc)) {
    return;
  }

  glDeleteShader(vs_handle);
  glDeleteShader(fs_handle);
}

void ShaderProgram::use() {
  glUseProgram(handle);
}

GLint ShaderProgram::get_uniform_location(std::string_view name) const {
  return glGetUniformLocation(handle, std::string(name).c_str());
}

void ShaderProgram::set_bool(std::string_view name, bool value) const {
  glUniform1i(get_uniform_location(name), static_cast<int>(value));
}

void ShaderProgram::set_int(std::string_view name, int value) const {
  glUniform1i(get_uniform_location(name), value);
}

void ShaderProgram::set_float(std::string_view name, float value) const {
  glUniform1f(get_uniform_location(name), value);
}
}

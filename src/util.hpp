#pragma once

#include <filesystem>
#include <optional>
#include <source_location>
#include <string_view>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace lgl::util {

std::optional<GLFWwindow*> create_window(int width, int height);

bool check_shader_compile_status(GLuint shader, std::source_location& src_loc);
bool check_shader_prog_link_status(GLuint shader_prog, std::source_location& src_loc);

std::filesystem::path resolve_texture(const std::string_view rel_path);

}

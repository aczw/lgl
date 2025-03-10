#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <optional>
#include <source_location>
#include <string_view>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace lgl::util {

constexpr glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
constexpr glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
constexpr glm::vec3 z_axis(0.0f, 0.0f, 1.0f);

std::optional<GLFWwindow*> create_window(int width, int height);

bool check_shader_compile_status(GLuint shader, const std::source_location& src_loc);
bool check_shader_program_link_status(GLuint shader_prog, const std::source_location& src_loc);

/**
 * Returns the absolute file path to a texture file.
 *
 * @param rel_path path relative to the hardcoded src/textures folder.
 * @return fs::path path object representing the canonical file path of the image.
 */
std::filesystem::path resolve_texture(std::string_view rel_path);

}

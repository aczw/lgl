#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <source_location>
#include <string_view>
#include <type_traits>

namespace lgl {

class ShaderProgram {
 private:
  GLuint handle;

 public:
  /**
   * Compiles and links a vertex and fragment shader together into a shader program object. By
   * giving @param src_loc a default value, it "customizes" it to equal where the call site is,
   * instead of this header file.
   *
   * "Base directory" refers to the folder of the file where this is being called.
   *
   * @param rel_vs_path relative path to the vertex shader from the base directory
   * @param rel_fs_path relative path to the fragment shader from the base directory
   * @param src_loc source location info
   */
  ShaderProgram(std::string_view rel_vs_path,
                std::string_view rel_fs_path,
                std::source_location src_loc = std::source_location::current());

  void use();

  /**
   * Gets a handle to the uniform variable in this shader program. This function works without
   * calling use().
   *
   * @param name name of the uniform variable
   * @return GLint handle to the variable
   */
  GLint get_uniform_location(std::string_view name) const;

  void set_bool(std::string_view name, bool value) const;
  void set_int(std::string_view name, int value) const;
  void set_float(std::string_view name, float value) const;

  /**
   * Generalized uniform setter.
   *
   * @tparam Ty type of the value
   * @param name name of the uniform variable
   * @param value value we're setting the variable to
   */
  template <typename Ty>
  void set_uniform(std::string_view name, const Ty& value) const {
    GLint unif_loc = get_uniform_location(name);

    if constexpr (std::is_same_v<Ty, bool>) {
      glUniform1i(unif_loc, static_cast<GLint>(value));
    } else if constexpr (std::is_same_v<Ty, GLint>) {
      glUniform1i(unif_loc, value);
    } else if constexpr (std::is_same_v<Ty, GLfloat>) {
      glUniform1f(unif_loc, value);
    } else if constexpr (std::is_same_v<Ty, glm::mat4>) {
      glUniformMatrix4fv(unif_loc, 1, GL_FALSE, glm::value_ptr(value));
    } else {
      static_assert(false, "Received an invalid type. Cannot convert to GLSL type");
    }
  }
};

}

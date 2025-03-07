#pragma once

#include <glad/glad.h>
#include <source_location>
#include <string>
#include <string_view>

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
  GLint get_uniform_location(const std::string& name) const;

  void set_bool(const std::string& name, bool value) const;
  void set_int(const std::string& name, int value) const;
  void set_float(const std::string& name, float value) const;
};

}

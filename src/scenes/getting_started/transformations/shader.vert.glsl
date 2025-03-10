#version 460 core

uniform mat4 u_Trans;

layout (location = 0) in vec3 vs_Pos;
layout (location = 1) in vec3 vs_Col;
layout (location = 2) in vec2 vs_UV;

out vec4 fs_Col;
out vec2 fs_UV;

void main() {
  fs_Col = vec4(vs_Col, 1.0);
  fs_UV = vs_UV;

  gl_Position = u_Trans * vec4(vs_Pos, 1.0);
}
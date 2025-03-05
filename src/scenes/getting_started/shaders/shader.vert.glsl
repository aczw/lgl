#version 460 core

layout (location = 0) in vec3 vs_Pos;
layout (location = 1) in vec3 vs_Col;

out vec4 fs_Col;

void main() {
  fs_Col = vec4(vs_Col, 1.0);
  gl_Position = vec4(vs_Pos, 1.0);
}
#version 460 core

uniform sampler2D u_Texture;

in vec4 fs_Col;
in vec2 fs_UV;

out vec4 out_Col;

void main() {
  out_Col = texture(u_Texture, fs_UV) * fs_Col;
}
#version 460 core

uniform sampler2D tex_0;
uniform sampler2D tex_1;

in vec4 fs_Col;
in vec2 fs_UV;

out vec4 out_Col;

void main() {
  out_Col = mix(texture(tex_0, fs_UV), texture(tex_1, fs_UV), 0.2);
}
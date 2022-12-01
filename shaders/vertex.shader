#version 330 core

// position and color
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texcoord;

out vec3 vertex_color;
out vec2 tex_coord;

void
main() {
  gl_Position = vec4(a_pos, 1.0);
  vertex_color = a_color;
  tex_coord = a_texcoord;
}

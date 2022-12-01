#version 330 core
in vec3 vertex_color;
in vec2 tex_coord;

out vec4 FragColor;

uniform sampler2D our_texture;

void main() {
  FragColor = texture(our_texture, tex_coord);
}

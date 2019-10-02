#version 330 core

/* Unused shader */

in vec2 UV;
out vec4 color;

uniform sampler2D texture0;
uniform int height;
uniform int width;
uniform float anim;

void main() {
  color = texture(texture0, UV);
}

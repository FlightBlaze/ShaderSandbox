#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texture0;
uniform float anim;

void main() {
    vec2 UV2 = UV;
    color = texture(texture0, UV2);
}

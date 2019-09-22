#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
    vec4 first = texture(texture0, UV);
    vec4 second = texture(texture1, UV);
    color = mix(first, vec4(second.rgb, 1), second.a);
}


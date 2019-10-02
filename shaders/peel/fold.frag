#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texture0;
uniform int height;
uniform int width;
uniform float anim;

const float radius = 0.01;
const float depth = 0.15;

void main() {
    /* Round the fold */
    
    float z = 0;
    
    if((UV.x + UV.y) / 2 < 1 - anim && (UV.x + UV.y) / 2 > 1 - anim - radius)
        z = (1 - abs((UV.x + UV.y) / 2 - (1 - anim)) / radius) * depth;
    
    /* Interpolate */
    
    color = texture(texture0, UV + vec2(pow(z, 2)));
}

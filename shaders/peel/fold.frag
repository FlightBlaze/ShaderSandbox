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
    
    if(anim <= 0.01)
        z *= anim * 100;
    
    /* Interpolate */
    
    vec2 UV2 = UV + vec2(pow(z, 2));
    
    if(UV2.x > 1 || UV2.y > 1)
        discard;
    
    color = texture(texture0, UV2);
}

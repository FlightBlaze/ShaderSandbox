#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texture0;
uniform int height;
uniform int width;
uniform float anim;

const vec4 background = vec4(0, 0, 0, 0);

void darkenBorder(float thickness, float intensity) {
    if(((UV.y * height + thickness) / height + (UV.x * width + thickness) / width ) / 2 > 1 - anim)
        color.a -= intensity;
}

void main() {
    if(anim == 1)
        discard;

    color = background;
    
    float anim1 = (anim * width + 1) / width;
    
    float mirrorX = (1 - anim1 * 2) - UV.x;
    float mirrorY = (1 - anim1 * 2) - UV.y;
    
    if(mirrorY >= 0 || mirrorX >= 0)
        return;
    
    vec4 backface = texture(texture0, vec2(mirrorY, mirrorX));
    
    color = backface;
    
    // add light
    color.rgb += 0.09 * backface.a;
    color.rgb += ((1 - anim - (UV.x + UV.y) / 2) / 2) * backface.a;
    
    if((UV.x + UV.y) / 2 < 1 - anim && (UV.x + UV.y) / 2 > 1 - anim - 0.02)
        color.rgb += ((1 - abs((UV.x + UV.y) / 2 - (1 - anim)) / 0.02) / 5.5) * backface.a;
        
    // antialias border
    darkenBorder(0.25, 0.2);
    darkenBorder(0.5, 0.3);
    darkenBorder(0.75, 0.2);
    darkenBorder(1, 0.1);
}

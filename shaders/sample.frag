#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texture0;
uniform int height;
uniform int width;
uniform float anim;

const vec4 background = vec4(0, 0, 0, 0);

void darkenBorder(float thickness, float intensity) {
    if((UV.x + (UV.y * height + thickness) / height ) / 2 > 1 - anim)
        color.a -= intensity;
}

void main() {
    if((UV.x + UV.y) / 2 > 1 - anim) {
        color = background;
        return;
    }
    
    color = texture(texture0, UV);
    
    if((UV.x + UV.y) / 2 <= 1 - anim * 2)
        return;
    
    float mirrorX = (1 - anim * 2) - UV.x;
    float mirrorY = (1 - anim * 2) - UV.y;
    
    if(mirrorY >= 0 || mirrorX >= 0)
        return;
    
    vec4 backface = texture(texture0, vec2(mirrorY, mirrorX));
    
    color.rgba = mix(color.rgba, vec4(backface.rgb, 1), backface.a);
    
    if(backface.a == 0)
        return;
    
    // add light
    color.rgb += 0.09 * backface.a;
    color.rgb += ((1 - anim - (UV.x + UV.y) / 2) / 2) * backface.a;
    
    if((UV.x + UV.y) / 2 < 1 - anim && (UV.x + UV.y) / 2 > 1 - anim - 0.02)
        color.rgb += ((1 - abs((UV.x + UV.y) / 2 - (1 - anim)) / 0.02) / 5.5) * backface.a;
        
    // antialias border
    darkenBorder(0.25, 0.75);
    darkenBorder(0.5, 0.5);
    darkenBorder(0.75, 0.333);
    darkenBorder(1, 0.25);
}

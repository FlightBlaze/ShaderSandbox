#version 330 core

in vec3 ourColor;
in vec2 UV;

out vec4 color;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform float ratio;
uniform float anim;

const vec4 background = vec4(0.6, 0.6, 0.6, 1);

void main() {

    if((UV.x + UV.y / ratio) > anim + anim / ratio)
        color = vec4(1, 0, 0, 1);
    else
        color = vec4(1, 1, 1, 1);

    //////////////////////////////////////////////
    return;
    
    if((UV.x + UV.y) / 2 > 1 - anim) {
        color = background;
        return;
    }
    
    if((UV.x + UV.y) / 2 <= 1 - anim * 2) {
        color = texture(ourTexture1, UV);
        return;
    }
    
    float mirrorX = (1 - anim * 2) - UV.x;
    float mirrorY = (1 - anim * 2) - UV.y;
    
    if(mirrorY >= 0 || mirrorX >= 0) {
        color = texture(ourTexture1, UV);
        return;
    }
    
    color = texture(ourTexture1, vec2(mirrorY, mirrorX));
    
    color += vec4(0.11, 0.11, 0.11, 0);
    
    float light = (1 - anim - (UV.x + UV.y) / 2) / 2;
    color += vec4(light, light, light, 0);
    
    if((UV.x + UV.y) / 2 < 1 - anim && (UV.x + UV.y) / 2 > 1 - anim - 0.02) {
        float add = (1 - abs((UV.x + UV.y) / 2 - (1 - anim)) / 0.02) / 4;
        color += vec4(add, add, add, 1);
    }
}

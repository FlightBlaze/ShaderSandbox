#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texture0;
uniform int height;
uniform int width;
uniform float anim;

const vec4 background = vec4(0, 0, 0, 0);

void main() {
    if((UV.x + UV.y) / 2 > 1 - anim) {
        color = background;
        return;
    }
    
    color = texture(texture0, UV);
    
    if(anim == 1)
        discard;
        
    // leave some space in bend
    if(((UV.x * width + 1) / width + (UV.y * height + 1) / height ) / 2 > 1 - anim)
        color.a = 0;
}

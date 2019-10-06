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
    
    if(anim <= 0.01)
        return;
        
    /* Leave some space in fold to not cover semi-transparent
       pixels in adhered texture */
       
    if(((UV.x * width + 1) / width + (UV.y * height + 1) / height ) / 2 > 1 - anim)
        color.a = 0;
}

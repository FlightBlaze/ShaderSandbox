#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texture0;
uniform int height;
uniform int width;
uniform float anim;

const vec4 trasparent = vec4(0, 0, 0, 0);

void darkenFold(float thickness, float intensity) {
    if(((UV.y * height + thickness) / height + (UV.x * width + thickness) / width ) / 2 > 1 - anim)
        color.a -= intensity;
}

void main() {
    if(anim <= 0.005)
        discard;

    color = trasparent;
    
    /* Animation value with 1 pixel shift to cover
       antialiased bend */
       
    float anim1 = (anim * width + 1) / width;
    
    /* Get the mirror pixel using inverted distance of current pixel from skyline */
    
    float skyline = 1 - anim1 * 2;
    vec2 mirror = vec2(skyline - UV.y, skyline - UV.x);
    
    /* Cut overlapping */
    
    if(mirror.x >= 0 || mirror.y >= 0 || mirror.y < -1 || mirror.x < -1)
        return;
        
    if((UV.x + UV.y) / 2 > 1 - anim)
        return;
    
    /* Interpolate */
    
    color = texture(texture0, mirror);
    
    /* Add light */
    
    color.rgb += 0.09 * color.a;
    color.rgb += (1 - anim - (UV.x + UV.y) / 2) / 2 * color.a;
    
    if((UV.x + UV.y) / 2 < 1 - anim && (UV.x + UV.y) / 2 > 1 - anim - 0.02)
        color.rgb += ((1 - abs((UV.x + UV.y) / 2 - (1 - anim)) / 0.02) / 5.5) * color.a;
        
    /* Antialias fold my making subpixels more transparent
       based on it's distance from fold line */
       
    darkenFold(0.25, 0.2);
    darkenFold(0.5, 0.3);
    darkenFold(0.75, 0.2);
    darkenFold(1, 0.1);
}

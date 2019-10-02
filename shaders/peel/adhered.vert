#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;

uniform int height;
uniform int width;

out vec2 UV;

void main() {
    vec3 pos = position;
        /*
    if(pos.y == -1 && width > height)
        pos.y *= (width / height) * 2;
        
    if(pos.x == -1 && width < height)
        pos.x *= height / width;
        */
    gl_Position = vec4(pos, 1.0f);
    UV = uv;
}

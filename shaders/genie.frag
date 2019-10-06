#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texture0;
uniform int height;
uniform int width;
uniform float anim;

const vec4 transparent = vec4(0, 0, 0, 0);

vec4 getColor(vec2 uv) {
    if (uv.x >= 0.001 && uv.x <= 0.99 && uv.y >= 0.001 && uv.y <= 0.99) {
      return vec4(texture(texture0, uv));
    }
    return transparent;
}

void main() {

    vec2 target = vec2(0.0, 0.0);
    vec2 pos = UV + normalize(UV - target) * (anim);
    
	color = getColor(pos);
}

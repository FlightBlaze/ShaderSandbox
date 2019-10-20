#version 330 core

in vec2 inCoord;
out vec4 outColor;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float uTime;
uniform vec2 uSize;

const vec4 transparent = vec4(0.0, 0.0, 0.0, 0.0);

vec4 clip(vec4 color, vec2 uv) {
    if (uv.x >= -0.0001 && uv.x <= 0.999 &&
	uv.y >= -0.0001 && uv.y <= 0.999) {
    	return color;
    }
    return transparent;
}

vec2 scale(vec2 uv, float anim) {
    return uv * (2 * anim + 1) - 2 * anim;
}

vec4 background(vec2 uv, float anim) { 
	return texture(uTexture0, uv);
}

vec4 foreground(vec2 uv, float anim) { 
	vec2 nuv = scale(uv, anim);
	return clip(texture(uTexture1, nuv), nuv);
}

vec4 blend(vec4 dst, vec4 src) {
	float alpha = src.a + dst.a * (1.0 - src.a);
    return vec4(
        (src.rgb * src.a + dst.rgb * dst.a * (1.0 - src.a)) / alpha,
        alpha
    );
}

vec4 adhered(vec2 uv, float anim) {
    if((uv.x + uv.y) / 2.0 > 1.0 - anim)
        return transparent;

    /* Leave some space in fold to not cover semi-transparent
       pixels in adhered texture */
       
    if ((uv.x * uSize.x + 1.0) / uSize.x / 2.0
	  + (uv.y * uSize.y + 1.0) / uSize.y / 2.0
	  > 1 - anim && anim > 0.01)
			return transparent;

    return foreground(uv, anim);
}

void main() {
	float anim = sin(uTime) / 2.0 + 0.5;

	outColor = blend(
		background(inCoord, anim),
		adhered(inCoord, anim)
	);
}

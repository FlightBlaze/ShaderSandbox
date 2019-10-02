#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D texDst;
uniform sampler2D texSrc;

void main() {
    vec4 dst = texture2D(texDst, UV);
    vec4 src = texture2D(texSrc, UV);

    float finalAlpha = src.a + dst.a * (1.0 - src.a);
    color = vec4(
        (src.rgb * src.a + dst.rgb * dst.a * (1.0 - src.a)) / finalAlpha,
        finalAlpha
    );
}


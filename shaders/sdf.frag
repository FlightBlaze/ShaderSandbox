#version 330 core

in vec2 UV;
out vec4 Color;

uniform float uTime;
uniform vec2 uMouse;
uniform vec2 uSize;

float smin(float a, float b, float k) {
	float h = max(k - abs(a - b), 0.0);
	return min(a, b) - h * h / (k * 4.0);
}

float sdElipsoid(vec3 pos, vec3 rad) {
	float k0 = length(pos / rad);
	float k1 = length(pos / rad / rad);
	return k0 * (k0 - 1.0) / k1;
}

float sdGuy(vec3 pos) {
	float t = 0.5;//fract(uTime);

	float dy = 4.0 * (1.0 - 2.0 * t);
	float y = 4.0 * t * (1.0 - t);
	
	vec2 u = normalize(vec2(1.0, -dy));
	vec2 v = vec2(dy, 1.0);

	vec3 center = vec3(0.0,	y, 0.0);

	float sy = 0.5 + 0.5 * y;
	float sz = 1.0 / sy;

	vec3 q = pos - center;
	q.yz = vec2(dot(u, q.yz), dot(v, q.yz));

	vec3 rad = vec3(0.25, 0.25 * sy, 0.25 * sz);

	float d = sdElipsoid(q, rad);

	vec3 h = q - vec3(0.0, 0.28, 0.0);
	float d2 = sdElipsoid(h, vec3(0.2));

	d = smin(d, d2, 0.03);

	return d;
}

float map(vec3 pos) {
	float d1 = sdGuy(pos);



	float d2 = pos.y - (-0.25);	

	return min(d1, d2);
}

vec3 calcNormal(vec3 pos) {
	vec2 e = vec2(0.0001, 0.0);

	return normalize(
		vec3(
			map(pos+e.xyy) - map(pos-e.xyy),
			map(pos+e.yxy) - map(pos-e.yxy),
			map(pos+e.yyx) - map(pos-e.yyx)
		)
	);
}

float castRay(vec3 ro, vec3 rd) {
	float t = 0.0;
	for(int i = 0; i < 100; i++) {
		vec3 pos = ro + t * rd;

		float h = map(pos);
		if( h < 0.001)
			break;

		t += h;
		if(t > 20.0)
			break;
	}
	if(t > 20.0) t = -1.0;
	return t;
}

void main() {
	vec2 p = (2.0 * UV * uSize - uSize.xy) / uSize.y;

	float an = 10.0 * uMouse.x;

	vec3 ta = vec3(0.0, 0.95, 0.0);
	vec3 ro = ta + vec3(1.5 * sin(an), 0.0, 1.5 * cos(an));
	vec3 ww = normalize(ta - ro);
	vec3 uu = normalize(cross(ww, vec3(0.0, 1.0, 0.0)));
	vec3 vv = normalize(cross(uu, ww));
	vec3 rd = normalize(
			vec3(p.x * uu + p.y * vv + 1.8 * ww)
	);

	vec3 col = mix(
			vec3(0.4, 0.75, 1.0) - 0.7 * rd.y,
			vec3(0.7, 0.75, 0.8),
			exp(-10.0 * rd.y)
	);	

	float t = castRay(ro, rd);

	if( t > 0.0) {
		vec3 pos = ro + t * rd;

		// normal
		vec3 nor = calcNormal(pos);

		vec3 mate = vec3(0.18);

		vec3 sun_dir = normalize(vec3(0.8, 0.4, 0.2));

		// diffuse
		float sun_diff = clamp(dot(nor, sun_dir), 0.0, 1.0); 
		float sky_diff = clamp( 0.5 + 0.5 * dot(nor,
					vec3(0.0, 1.0, 0.0)), 0.0, 1.0
		); 
		float bou_diff = clamp( 0.5 + 0.5 * dot(nor,
					vec3(0.0, -1.0, 0.0)), 0.0, 1.0
		);
		// shadow
		float sun_sha = step(castRay(
				pos + nor * 0.001, sun_dir), 0.0);
		

		col  = mate * vec3(7.0, 4.5, 3.0) * sun_diff * sun_sha;
		col += mate *vec3(0.5, 0.8, 0.9) * sky_diff;
		col += mate *vec3(0.7, 0.3, 0.2) * bou_diff;

	}

	col = pow(col, vec3(0.4545));

	Color = vec4(col, 1.0);
}

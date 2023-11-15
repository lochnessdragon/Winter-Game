// version directive handled by shader class
precision highp float;

uniform sampler2D msdfAtlas;
uniform vec4 textColor;
//uniform float pxRange; // set to distance field's pixel range set to 2 by font class

in vec2 uv;

out vec4 FragColor;

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

float screenPxRange() {
    vec2 unitRange = vec2(2)/vec2(textureSize(msdfAtlas, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(uv);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main() {
	vec3 msd = texture(msdfAtlas, uv).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange() * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	FragColor = mix(vec4(0.0f, 0.0f, 0.0f, 0.0f), textColor, opacity);
}
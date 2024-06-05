// version directive handled by shader class
precision highp float;

#define MAX_TEXTURES 32
uniform sampler2D spriteTextures[MAX_TEXTURES];

in vec2 uv;
in vec4 color;
in float texIndex;

out vec4 FragColor;

void main() {
	vec4 texColor = color;
	switch(int(texIndex))
	{
		case  0: texColor *= texture(spriteTextures[ 0], uv); break;
		case  1: texColor *= texture(spriteTextures[ 1], uv); break;
		case  2: texColor *= texture(spriteTextures[ 2], uv); break;
		case  3: texColor *= texture(spriteTextures[ 3], uv); break;
		case  4: texColor *= texture(spriteTextures[ 4], uv); break;
		case  5: texColor *= texture(spriteTextures[ 5], uv); break;
		case  6: texColor *= texture(spriteTextures[ 6], uv); break;
		case  7: texColor *= texture(spriteTextures[ 7], uv); break;
		case  8: texColor *= texture(spriteTextures[ 8], uv); break;
		case  9: texColor *= texture(spriteTextures[ 9], uv); break;
		case 10: texColor *= texture(spriteTextures[10], uv); break;
		case 11: texColor *= texture(spriteTextures[11], uv); break;
		case 12: texColor *= texture(spriteTextures[12], uv); break;
		case 13: texColor *= texture(spriteTextures[13], uv); break;
		case 14: texColor *= texture(spriteTextures[14], uv); break;
		case 15: texColor *= texture(spriteTextures[15], uv); break;
		case 16: texColor *= texture(spriteTextures[16], uv); break;
		case 17: texColor *= texture(spriteTextures[17], uv); break;
		case 18: texColor *= texture(spriteTextures[18], uv); break;
		case 19: texColor *= texture(spriteTextures[19], uv); break;
		case 20: texColor *= texture(spriteTextures[20], uv); break;
		case 21: texColor *= texture(spriteTextures[21], uv); break;
		case 22: texColor *= texture(spriteTextures[22], uv); break;
		case 23: texColor *= texture(spriteTextures[23], uv); break;
		case 24: texColor *= texture(spriteTextures[24], uv); break;
		case 25: texColor *= texture(spriteTextures[25], uv); break;
		case 26: texColor *= texture(spriteTextures[26], uv); break;
		case 27: texColor *= texture(spriteTextures[27], uv); break;
		case 28: texColor *= texture(spriteTextures[28], uv); break;
		case 29: texColor *= texture(spriteTextures[29], uv); break;
		case 30: texColor *= texture(spriteTextures[30], uv); break;
		case 31: texColor *= texture(spriteTextures[31], uv); break;
	}
	if (texColor.a < 0.01)
		discard;
	FragColor = texColor;
}
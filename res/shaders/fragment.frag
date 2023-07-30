// version directive handled by shader class
precision highp float;

in vec2 uv;
in vec3 normal;
in vec3 fragPosViewSpace;
in vec3 lightPosViewSpace;

// lights
//  - we perform all lighting calculations in view space so that the camera is located at 0, 0
//  - we should look into a way to support the x nearest lights
uniform vec3 ambientLightColor;
uniform float ambientLightStrength;

uniform vec3 lightColor;
uniform float lightStrength;
uniform float specularStrength;

// textures
uniform sampler2D texture0;

out vec4 FragColor;

void main() {
	vec4 objectColor = texture(texture0, uv);

	// apply ambient lighting
	vec3 ambientColor = ambientLightStrength * ambientLightColor;

	// apply diffuse lighting
	vec3 normalNormalized = normalize(normal);
	vec3 surfToLight = normalize(lightPosViewSpace - fragPosViewSpace);
	float diffuseDifference = max(dot(normalNormalized, surfToLight), 0.0);
	vec3 diffuseColor = diffuseDifference * lightColor * lightStrength;

	// apply specular highlights
	vec3 surfToCam = normalize(vec3(0.0, 0.0, 0.0) - fragPosViewSpace);
	vec3 reflectDir = reflect(-surfToLight, normalNormalized);
	float specularAmount = pow(max(dot(surfToCam, reflectDir), 0.0), 32);
	vec3 specularColor = specularStrength * specularAmount * lightColor;

	objectColor *= vec4(ambientColor + diffuseColor + specularColor, 1.0);

    FragColor = objectColor;
}
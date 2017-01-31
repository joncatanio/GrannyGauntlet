#version 330 core

in vec3 fragNor;
in vec3 worldPos;

uniform vec3 lightPos;
uniform vec3 lightClr;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpc;
uniform float MatShiny;

uniform mat4 M;
uniform mat4 V;

out vec4 color;

void main() {
	vec3 fragNormal = normalize(fragNor);
	vec3 lightDir = normalize((V * vec4(lightPos, 1.0)).xyz - worldPos);
	vec3 reflectVec = reflect(-lightDir, fragNormal);
	vec3 viewVec = normalize(-worldPos);

	float lightNormalDot = max(dot(lightDir, fragNormal), 0.0);
	float specularValue = 0.0;
	if (lightNormalDot > 0.0) {
		float specWeight = max(dot(reflectVec, viewVec), 0.0);
		specularValue = pow(specWeight, MatShiny);
	}

	vec4 diffuseClr = vec4(1.0, 0.3, 0.3, 1.0) * max(lightNormalDot, 0) * vec4(lightClr, 1.0);
	vec4 specularClr = specularValue * vec4(MatSpc, 1.0) * vec4(lightClr, 1.0);
	vec4 ambientClr = vec4(MatAmb, 1.0);

	color = diffuseClr + specularClr + ambientClr;
}

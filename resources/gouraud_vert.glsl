#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 MV;

uniform vec3 lightPos;
uniform vec3 lightClr;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpc;
uniform float MatShiny;

uniform int UseNorClr;

out vec4 clr;
out vec3 lightVert;

void main() {
	vec3 worldPos = (MV * vertPos).xyz;
	lightVert = lightPos - worldPos;

	vec3 vertNormal = normalize(vertNor);
	vec3 lightNor = normalize(lightVert);

	gl_Position = P * MV * vertPos;

	vec3 fragNor;

	fragNor = (MV * vec4(vertNormal, 0.0)).xyz;

	float lightDot = dot(fragNor, lightNor);

	vec3 R = normalize(-lightNor + 2 * dot(fragNor, lightNor) * fragNor);
	vec3 V = normalize(-worldPos);

	// Gouraud diffuse and specular lighting
	if (UseNorClr == 0) {
		clr = (vec4(MatDif, 1.0) * max(lightDot, 0) * vec4(lightClr, 1.0) + vec4(MatSpc, 1.0) * pow(max(dot(V, R), 0), MatShiny) * vec4(lightClr, 1.0)) + vec4(MatAmb, 1.0);
	} else {
		clr = (vec4(fragNor, 1.0) * max(lightDot, 0) * vec4(lightClr, 1.0) + vec4(fragNor, 1.0) * pow(max(dot(V, R), 0), MatShiny) * vec4(lightClr, 1.0)) + vec4(fragNor, 1.0);
	}
}
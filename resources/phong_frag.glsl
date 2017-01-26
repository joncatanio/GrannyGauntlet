#version 330 core

in vec3 fragNor;
in vec3 lightVert;
in vec3 worldPos;

uniform vec3 lightClr;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpc;
uniform float MatShiny;

uniform mat4 M;

out vec4 color;

void main() {
	vec3 fragNormal = normalize(fragNor);
	vec3 lightNormal = normalize(lightVert);

	float lightDot = dot(fragNor, lightNormal);

	vec3 R = normalize(-lightNormal + 2 * dot(fragNor, lightNormal) * fragNor);
	vec3 V = normalize(-worldPos);

	//vec3 viewVec = normalize((-V * vec4(worldPos, 0.0)).xyz);

	vec4 newClr;

	// Phong diffuse and specular lighting
	newClr = (vec4(MatDif, 1.0) * max(lightDot, 0) * vec4(lightClr, 1.0) + vec4(MatSpc, 1.0) * pow(max(dot(V, R), 0), MatShiny) * vec4(lightClr, 1.0)) + vec4(MatAmb, 1.0);

	color = newClr;
}

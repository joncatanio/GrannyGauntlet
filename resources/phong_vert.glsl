#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform mat4 tiM;

out vec3 fragNor;
out vec3 worldPos;

void main() {

	// Calculate the position of the vertex in view space
	worldPos = (V * M * vertPos).xyz;

	// Calculate the normal of the vertex in world space
	fragNor = normalize((tiM * vec4(normalize(vertNor), 0.0)).xyz);

	// Set the position of the vertex in perspective space
	gl_Position = P * V * M * vertPos;
}

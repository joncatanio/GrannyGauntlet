#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

uniform vec3 lightPos;

out vec3 fragNor;
out vec3 lightVert;
out vec3 worldPos;

void main() {
	worldPos = (V * M * vertPos).xyz;
	lightVert = (V * vec4(lightPos, 1.0)).xyz;

	lightVert = normalize(lightVert - worldPos);

	vec3 vertNormal = normalize(vertNor);

	fragNor = normalize((V * M * vec4(vertNormal, 0.0)).xyz);

	gl_Position = P * V * M * vertPos;
}
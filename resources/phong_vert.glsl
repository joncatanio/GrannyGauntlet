#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform mat4 tiM;

// Push the position in camera space and normal in world space
// to the fragment shader
out vec3 positionInCamSpace;
out vec3 normalInWorldSpace;
out vec2 texCoord;

void main() {

	// Set the position of the vertex in homogeneous space
	gl_Position = P * V * M * vertPos;

	// Calculate the position of the vertex in camera/view space
	positionInCamSpace = (V * M * vertPos).xyz;

	// Calculate the normal of the vertex in world space
	normalInWorldSpace = normalize((tiM * vec4(normalize(vertNor), 0.0)).xyz);

	texCoord = vertTex;
}

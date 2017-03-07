#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec2 vertTex;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
out vec2 texCoord;

void main() {

	// Set the position of the vertex in homogeneous space
	gl_Position = P * (V * M * vec4(0.0, 0.0, 0.0, 1.0) + vec4(vertPos.xy, 0.0, 1.0));

	// texture coordinates
	texCoord = vertTex;
}

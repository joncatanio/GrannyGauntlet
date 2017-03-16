#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec2 vertTex;

uniform mat4 P;

// Camera space sans rotation (the magic that causes the image to billboard)
uniform mat4 billboardTransform;

out vec2 texCoord;

void main() {

	// Set the position of the vertex to always be relative to the camera
	gl_Position = P * billboardTransform * vec4(vertPos.xy, 0.0, 1.0);

	// Resize to be in texel space (0, 1)
	vec2 vertTemp = vertPos.xy * 0.5 + 0.5;
	texCoord = vertTemp;
}

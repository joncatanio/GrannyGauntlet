#version 330 core

in vec3 vTexCoord;

uniform samplerCube cubemap;

out vec4 color;

void main() {
 
	color = texture(cubemap, vTexCoord);
	//color = vec4(1.0, 0.3, 0.3, 1.0);
}


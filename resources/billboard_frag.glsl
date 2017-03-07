#version 330 core

in vec3 vTexCoord;

uniform sampler2D billboardTex;

out vec4 color;

void main() {
 
	color = texture(billboardTex, vTexCoord.xy);
	//color = vec4(1.0, 0.3, 0.3, 1.0);
}


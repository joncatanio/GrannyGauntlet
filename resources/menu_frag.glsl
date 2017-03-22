#version 330 core

in vec2 texCoord;

uniform sampler2D textureMap;

out vec4 color;

void main() {
 
	vec4 colorTemp = texture(textureMap, texCoord);

	color = colorTemp;
}

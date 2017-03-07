#version 330 core

in vec2 texCoord;

uniform sampler2D textureMap;

out vec4 color;

void main() {
 
	vec4 colorTemp = texture(textureMap, texCoord);

	if (colorTemp.r >= 0.9 && colorTemp.g >= 0.9 && colorTemp.b >= 0.9) {
		discard;
	}

	color = colorTemp;
}

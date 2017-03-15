#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in float lifePercent;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out float life;

void main() {
	gl_Position = P * V * M * vertPos;
	life = lifePercent;
}

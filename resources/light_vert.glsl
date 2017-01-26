#version  330 core

layout(location = 0) in vec4 vertPos;

uniform mat4 P;
uniform mat4 MV;

void main()
{
	gl_Position = P * MV * vertPos;
}

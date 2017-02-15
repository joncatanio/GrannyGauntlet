#version  330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec3 vTexCoord;

void main() {

  gl_Position = P * V * M * vec4(vertPos.xyz, 1.0);

  vTexCoord = vertPos;  

}

#version 330 core

in vec3 vTexCoord;

<<<<<<< 678a7567b1118afe499b003031fbbe261ca9cc44
uniform sampler2D billboardTex;
=======
uniform samplerCube cubemap;
>>>>>>> Initial Billboard implementation with 3d objects

out vec4 color;

void main() {
	color = texture(billboardTex, vTexCoord.xy);
	//color = vec4(1.0, 0.3, 0.3, 1.0);
}


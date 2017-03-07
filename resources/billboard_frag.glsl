#version 330 core

in vec3 vTexCoord;

<<<<<<< 9d205b09183c2c91a24a07870b944c46a1346576
<<<<<<< 678a7567b1118afe499b003031fbbe261ca9cc44
uniform sampler2D billboardTex;
=======
uniform samplerCube cubemap;
>>>>>>> Initial Billboard implementation with 3d objects
=======
uniform sampler2D billboardTex;
>>>>>>> Temporary billboard changes

out vec4 color;

void main() {
	color = texture(billboardTex, vTexCoord.xy);
	//color = vec4(1.0, 0.3, 0.3, 1.0);
}


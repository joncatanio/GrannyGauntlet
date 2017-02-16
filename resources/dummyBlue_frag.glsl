#version 330 core

#define MAX_POINT_LIGHTS 10
#define MAX_DIRECTION_LIGHTS 10
#define MAX_AREA_LIGHTS 10

struct Light {
	vec3 location;
	vec3 color;
	vec3 orientation;
};

in vec3 fragNor;
in vec3 worldPos;

uniform vec3 lightPos;
uniform vec3 lightClr;

// TODO(rgarmsen2295): Implement support for point lights
//uniform Light pointLights[MAX_POINT_LIGHTS];
//uniform int numPointLights;

// Currently accepts up to a max of 10 direction lights
uniform Light directionLights[MAX_DIRECTION_LIGHTS];
uniform int numDirectionLights;

// TODO(rgarmsen2295): Implement support for area lights
//uniform Light areaLights[MAX_AREA_LIGHTS];
//uniform int numAreaLights;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpc;
uniform float MatShiny;

uniform mat4 M;
uniform mat4 V;

out vec4 color;

vec3 dirLightColor(vec3 fragNormal, vec3 view) {
	vec3 dirLightColor = vec3(0.0);

	for (int i = 0; i < numDirectionLights; ++i) {
		vec3 lightColor = directionLights[i].color;
		vec3 lightDir = normalize(-directionLights[i].orientation);
		vec3 reflect = reflect(-lightDir, fragNormal);

		float lightNormalDot = max(dot(lightDir, fragNormal), 0.0);
		float specularValue = 0.0;
		if (lightNormalDot > 0.0) {
			float specWeight = max(dot(reflect, view), 0.0);
			specularValue = pow(specWeight, MatShiny);
		}

		vec3 diffuse = vec3(0.3, 0.3, 1.0) * max(lightNormalDot, 0.0) * lightColor;
		vec3 specular = specularValue * MatSpc * lightColor;
		dirLightColor += diffuse + specular;
	}

	vec3 ambient = MatAmb;
	dirLightColor += ambient;

	return dirLightColor;
}

void main() {
	vec3 fragNormal = normalize(fragNor);
	vec3 view = normalize(-worldPos);

	//vec3 pointLightColor = pointLightColor();
	vec3 directionalLightColor = dirLightColor(fragNormal, view);
	//vec3 areaLightColor = areaLightColor();

	color = vec4(directionalLightColor, 1.0);
}

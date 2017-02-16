#version 330 core

#define MAX_POINT_LIGHTS 10
#define MAX_DIRECTION_LIGHTS 10
#define MAX_AREA_LIGHTS 10

// Structure matches that CPU side in "ShaderManager.h" minus the light type
struct Light {
	vec3 location;
	vec3 color;
	vec3 orientation;
};

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

in vec3 positionInCamSpace;
in vec3 normalInWorldSpace;

out vec4 color;

// Calculates the total color (specular/diffuse) from directional lights
// Loops through up to |MAX_DIRECTIONAL_LIGHTS| with a soft cap set by |numDirectionLights|
vec3 dirLightColor(vec3 fragNormal, vec3 view) {
	vec3 dirLightColor = vec3(0.0);

	for (int i = 0; i < numDirectionLights; ++i) {
		vec3 lightColor = directionLights[i].color;
		vec3 lightDir = normalize(-directionLights[i].orientation);

		// How close are the object normal and the light's direction?
		float lightNormalDot = max(dot(lightDir, fragNormal), 0.0);

		// Calculate diffuse component from light
		float diffuseValue = max(lightNormalDot, 0.0);
		vec3 diffuse = diffuseValue * MatDif * lightColor;

		// Calculate specular component from light
		vec3 reflect = reflect(-lightDir, fragNormal);

		float specularValue = 0.0;

		// Don't want specular when the light vector is > 180 from the normal
		// (object is looking the wrong way)
		if (lightNormalDot > 0.0) {

			// How close are we to looking straight at the reflection vector?
			float specWeight = max(dot(reflect, view), 0.0);

			// Get shiny with it
			specularValue = pow(specWeight, MatShiny);
		}
		vec3 specular = specularValue * MatSpc * lightColor;

		// Add light color to total directional color
		dirLightColor += diffuse + specular;
	}


	return dirLightColor;
}

void main() {

	// Normalize the normal of the fragment in world space
	vec3 fragNormalInWorldSpace = normalize(normalInWorldSpace);

	// Gets the view direction by normalizing the negative position of the fragment in camera space
	// So, Camera - worldPosition, except in Camera space -> Camera = (0, 0, 0)
	vec3 view = normalize(-positionInCamSpace);

	// Calculate total specular/diffuse color contributions from all light types
	//vec3 pointLightColor = pointLightColor();
	vec3 directionalLightColor = dirLightColor(fragNormalInWorldSpace, view);
	//vec3 areaLightColor = areaLightColor();

	// Calculate ambient color
	vec3 ambient = MatAmb;

	// Calculate the total color
	color = vec4(directionalLightColor + ambient, 1.0);
}

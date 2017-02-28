#version 330 core
uniform sampler2D shadowMapTex;

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
in vec3 positionInLightSpace;


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
	// TODO(rgarmsen2295): vec3 pointLightColor = pointLightColor();
	vec3 directionalLightColor = dirLightColor(fragNormalInWorldSpace, view);
	// TODO(rgarmsen2295): vec3 areaLightColor = areaLightColor();

	// Calculate ambient color
	vec3 ambient = MatAmb;

	//sm
	float shadeFactor = 1.0;

	vec3 shifted = (positionInLightSpace + 1.0) / 2.0;

	float smTexOffset = 1.0/16384.0;
	float samples = 0.0;
	float shadedFrags = 0.0;

	for (float i = -3.0; i<=3.0; i++) {
		for (float j = -3.0; j<=3.0; j++) {
		        vec3 shiftedOffsetted = vec3(shifted.x + i * smTexOffset, shifted.y + j * smTexOffset, shifted.z);
    	    	if(shiftedOffsetted.x > 1.0 || shiftedOffsetted.x < 0.0 || shiftedOffsetted.y > 1.0 || shiftedOffsetted.y < 0.0 ) {
            	    //shadeFactor = 0.3;
            	} else {
                    float curDepth = shifted.z;
                    float smValue = texture(shadowMapTex, shiftedOffsetted.xy).r;

                    samples++;
                    if((smValue + 0.001) < curDepth) {
                           //shadeFactor = 0.5;
                           shadedFrags++;
                    }
                }
    	}
	}

	float PCFfac = shadedFrags / samples;
	shadeFactor = PCFfac * 0.5 + (1.0 -PCFfac) * 1.0;

    /*
	if(shifted.x > 1.0 || shifted.x < 0.0 || shifted.y > 1.0 || shifted.y < 0.0 ) {
	    shadeFactor = 0.3;
	} else {
        float curDepth = shifted.z;
        float smValue = texture(shadowMapTex, shifted.xy).r;

        if((smValue + 0.001) < curDepth) {
               shadeFactor = 0.5;
        }
    }
    */

	// Calculate the total color
	color = shadeFactor * vec4(directionalLightColor + ambient, 1.0);
	//color = vec4(vec3(smValue), 1.0);
}

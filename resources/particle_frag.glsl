#version 330 core
uniform sampler2D textureMap;

in float life;

out vec4 Outcolor;

void main()
{
	//float alpha = texture(alphaTexture, gl_PointCoord).r;
	//Outcolor = vec4(partCol.rgb, partCol.a*alpha);

	   vec2 pointCoord = gl_PointCoord * 0.25;
       float scale = floor(life * 16.0);
       //float scale = 0.0;

       if(scale  >= 12.0){
          pointCoord.y = pointCoord.y + 0.75;
       } else if(scale >= 8.0){
          pointCoord.y = pointCoord.y + 0.5;
       } else if(scale >= 4.0){
          pointCoord.y = pointCoord.y + 0.25;
       }

       if(mod(scale, 4) == 1){
          pointCoord.x = pointCoord.x + 0.25;
       } else if(mod(scale, 4) == 2){
          pointCoord.x = pointCoord.x + 0.5;
       } else if(mod(scale, 4) == 3){
          pointCoord.x = pointCoord.x + 0.75;
       }

    	float alpha = texture(textureMap, pointCoord).r;

    vec3 color = (1.0 - life) * vec3(0.0, 0.0, 1.0) + life * vec3(0.0, 1.0, 1.0);

	Outcolor = vec4(color, alpha);
}

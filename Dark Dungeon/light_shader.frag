uniform vec2 position;
uniform vec3 lightColor;
uniform float height; 
uniform float radius;
uniform float intensity;
uniform float angle;
uniform vec2 direction;
uniform sampler2D nMaps;

void main()
{		
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

	//Bunch of transformations (C++ -> GLSL)
	vec2 pixel = gl_FragCoord.xy;		
	pixel.y = height - pixel.y;

	vec2 aux = position - pixel;
	float d = length(aux);
	
	//Get the normal vector out of the normal texture's information
	vec3 normal = texture2D(nMaps, gl_TexCoord[0].st).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(normal);
	
	vec3 dd = vec3(aux.x, aux.y, 0.0);
	
	if(d <= radius) {			
        float desc = 1.0/d * intensity * 10.0;
		float shadows = max(dot(normalize(dd), normal), 0.0);				
		
		//Intensity gradient
		float granicaOpada = clamp((1.0 - d / radius), 0.0, 1.0);

		//I think it's for sprites (if their normal map is just white)
		vec3 condition = vec3(1.0, 1.0, 1.0);
		if(texture2D(nMaps, gl_TexCoord[0].st).rgb == condition)
			shadows = 0.5;

		//Sets shadow to 0 if out of the direction circle
		// if(dot(normalize(aux), direction) < angle*d/radius)
  //            shadows = 0.0;

 		color = vec4(desc * shadows, desc * shadows, desc * shadows, 1.0) * 
 				vec4(lightColor * granicaOpada, 1.0);		
	}		
	
	gl_FragColor = color;
}

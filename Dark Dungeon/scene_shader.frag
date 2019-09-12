uniform sampler2D geometry;
uniform sampler2D lightmap;
uniform vec3 environment;

void main() {		
	vec4 color;
	vec4 amb = vec4(environment,1.0);
	
	vec4 t1 = texture2D(geometry, gl_TexCoord[0].st);
	vec4 t2 = texture2D(lightmap, gl_TexCoord[0].st);
	
	color = t1*amb;
    color += t1*t2*3.0;

	gl_FragColor = color;
}

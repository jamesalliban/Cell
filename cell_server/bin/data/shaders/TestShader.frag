#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect baseMap;
varying vec3 vertex_light_position;
varying vec3 norm;
uniform float contrast;
uniform float brightness;
uniform float red;
uniform float green;
uniform float blue;
uniform float alpha;

void main(void) {

	/* Extract colors from baseMap */
	vec4 baseColor      = texture2DRect( baseMap, gl_TexCoord[0].st );
	vec3 normal			= normalize( norm );
	float diffuse_value = max(dot(normal, vertex_light_position), 0.0);
    vec4 alphaVec       = vec4(1, 1, 1, alpha);
	baseColor.rgb       *= diffuse_value;

    baseColor.rgb       *= contrast;
    baseColor.rgb       += brightness;
    baseColor           *= alpha;
	baseColor.r			+= red;
	baseColor.g			+= green;
	baseColor.b			+= blue;

	gl_FragColor	    =	baseColor;
}



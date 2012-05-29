#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect baseMap;
varying vec3 vertex_light_position;
varying vec3 norm;
uniform float contrast;
uniform float brightness;
uniform float alpha;
uniform float red;
uniform float green;
uniform float blue;

void main(void) {

	/* Extract colors from baseMap */
	vec4 baseColor      = texture2DRect( baseMap, gl_TexCoord[0].st );
	vec3 normal			= normalize( norm );
	float diffuse_value = max(dot(normal, vertex_light_position), 0.0);
    vec3 colourVec       = vec3(red, green, blue);

	baseColor.rgb       *= diffuse_value;

    baseColor.rgb       *= contrast;
    baseColor.rgb       += brightness;
    baseColor.rgb       += colourVec;
    if (baseColor.a > 0.9) baseColor.a = alpha;

    vec4 result = clamp(baseColor, 0.0, 1.0);

	gl_FragColor	    =	result;
}



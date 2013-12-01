#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect baseMap;
uniform float textAlphaAdd;

void main(void) 
{
	vec4 baseColor = texture2DRect( baseMap, gl_TexCoord[0].st );
	baseColor.a = clamp((((baseColor.r + baseColor.g + baseColor.b) / 3.0) + textAlphaAdd), 0.0, 1.0);
	gl_FragColor = baseColor;
}



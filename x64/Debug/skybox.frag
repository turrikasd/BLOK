#version 120
precision highp float;
 
in vec2 UV;
in vec3 normal;
in float tex;

uniform sampler2D ZP;
uniform sampler2D ZN;
uniform sampler2D XP;
uniform sampler2D XN;
uniform sampler2D YP;
uniform sampler2D YN;

void main(void) 
{
	if (normal.z > 0)
		gl_FragColor = texture2D(ZP, -UV);
	else if (normal.z < 0)
		gl_FragColor = texture2D(ZN, -UV);
	else if (normal.x > 0)
		gl_FragColor = texture2D(XP, -UV);
	else if (normal.x < 0)
		gl_FragColor = texture2D(XN, -UV);
	else if (normal.y > 0)
		gl_FragColor =  texture2D(YP, vec2(-UV.y, UV.x));
	else
		gl_FragColor = texture2D(YN, -UV);
}
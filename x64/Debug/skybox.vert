#version 120
in  vec3 in_Position;
in  vec2 in_UV;
in  vec3 in_Normal;

varying vec2 UV;
varying vec3 normal;
varying float tex;
 
uniform mat4 VP;
 
void main(void) 
{
	vec3 wPos = in_Position * 5000.0 + vec3(4096.0, 129.0, 4096.0);
    gl_Position = VP * vec4(wPos, 1.0);
	
	UV = in_UV;
	normal = in_Normal;
	
	tex = 1;
}
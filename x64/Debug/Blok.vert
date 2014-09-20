#version 120
attribute  vec3 in_Position;
attribute  vec2 in_UV;
attribute  vec3 in_Normal;
attribute  vec3 xyz;

varying vec3 local;
varying vec3 normal;
varying vec2 UV;
varying vec3 Position_worldspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;
varying vec4 ShadowCoord;
varying vec3 Normal_cameraspace;
varying float height;
varying float stone;

uniform mat4 V;
uniform vec3 LightInvDirection_worldspace;
uniform mat4 DepthBiasMVP;
uniform mat4 VP;

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product));// * 43758.5453);
}

void main() 
{
	local = in_Position;
	normal = in_Normal;
	
	vec3 wPos = in_Position + xyz;
    gl_Position = VP * vec4(wPos, 1.0);	
	
	ShadowCoord = DepthBiasMVP * vec4(wPos, 1);
	Position_worldspace = wPos;
	EyeDirection_cameraspace = vec3(0,0,0) - (V * vec4(wPos, 1)).xyz;
	
	LightDirection_cameraspace = (V * vec4(LightInvDirection_worldspace, 0)).xyz;
	
	Normal_cameraspace = ( V * vec4(in_Normal, 0)).xyz;
	
	UV = in_UV;
	float h = xyz.y / 50;
	height = 1.0 - (h * h);
	
	stone = random(vec3(floor(xyz.x), floor(xyz.y), floor(xyz.z)), 12) / 10 - height;
}
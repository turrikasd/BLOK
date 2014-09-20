#version 150

layout(points) in;
layout(points, max_vertices = 1) out;

in vec2 gUV[];
in vec3 gPosition_worldspace[];
in vec3 gNormal_cameraspace[];
in vec3 gEyeDirection_cameraspace[];
in vec3 gLightDirection_cameraspace[];
in vec4 gShadowCoord[];
in float gheight[];
in float gstone[];

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec4 ShadowCoord;
out float height;
out float stone;

void main() {
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	EndPrimitive();
	
	UV = gUV[0];
	Position_worldspace = gPosition_worldspace[0];
	Normal_cameraspace = gNormal_cameraspace[0];
	EyeDirection_cameraspace = gEyeDirection_cameraspace[0];
	LightDirection_cameraspace = gLightDirection_cameraspace[0];
	ShadowCoord = gShadowCoord[0];
	height = gheight[0];
	stone = gstone[0];
}
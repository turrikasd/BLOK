#version 150
in  vec3 in_Position;
in  vec2 vertexUV;
in  vec3 in_Normal;
//attribute int[3] bPos;

out vec2 UV;
out vec3 normal;
out float height;
out vec3 local;
out vec4 ShadowCoord;

// mvpmatrix is the result of multiplying the model, view, and projection matrices */
uniform mat4 MVP;
uniform vec3 BlockPos;
uniform mat4 DepthBiasMVP;
 
// We output the ex_Color variable to the next shader in the chain
void main(void) {
    // Since we are using flat lines, our input only had two points: x and y.
    // Set the Z coordinate to 0 and W coordinate to 1
 
	//vec3 newPos = vec3(BlockPos.x - 16384 / 16, BlockPos.y, BlockPos.z - 16384 / 16);
 
    gl_Position = MVP * 
	(vec4(BlockPos.x, BlockPos.y, BlockPos.z, 0.0) + vec4(in_Position.x, in_Position.y, in_Position.z, 1.0));
	//(vec4(in_Position.x + bPos[0], in_Position.y + bPos[2], in_Position.z + bPos[2], 1.0));
	
	normal = in_Normal;
    UV = vertexUV;
	height = BlockPos.y / 50;
	local = in_Position;
	
	ShadowCoord = DepthBiasMVP * (vec4(BlockPos.x, BlockPos.y, BlockPos.z, 0.0) + vec4(in_Position.x, in_Position.y, in_Position.z, 1.0));
}
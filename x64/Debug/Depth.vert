#version 150

// Input vertex data, different for all executions of this shader.
attribute vec3 in_Position;
attribute vec4 xyzt;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;

void main()
{
	//vec3 a = xyz.xyz;
	vec3 wPos = in_Position + xyzt.xyz;
	gl_Position = depthMVP * vec4(wPos, 1);
}


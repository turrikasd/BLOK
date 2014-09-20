#version 150
// It was expressed that some drivers required this next line to function properly
precision highp float;
 
in vec2 UV;
in vec3 normal;
in float height;
in vec3 local;
in vec4 ShadowCoord;

out vec4 gl_FragColor;

uniform sampler2D mainText;
uniform vec3 light;
uniform sampler2DShadow shadowMap;

void main(void) {

	vec3 difPos = abs(local * 2 - normal);
	
	float cX = 1.0 - difPos.x;
	cX *= 5.0;
	cX = min(cX, 1.0);
	
	float cY = 1.0 - difPos.y;
	cY *= 5.0;
	cY = min(cY, 1.0);
	
	float cZ = 1.0 - difPos.z;
	cZ *= 5.0;
	cZ = min(cZ, 1.0);
	
	float center = cX * cY * cZ;
	float lightDot = dot(normal , light);
	float cosTheta = clamp(lightDot, 0, 1);
	
	float visibility = texture(shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z)/ShadowCoord.w));
	
	//gl_FragColor = vec4(normal.x, normal.y, normal.z, 1.0);
	//gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0) * visibility;
	//gl_FragColor = vec4(0.4f, 0.6f, 0.4f, 1.0f) * height * (0.4 + cosTheta) * //texture(mainText, UV) * 
	//center * (visibility + 0.2);
	
	gl_FragColor = vec4(0.4f, 0.6f, 0.4f, 1.0f) * height * (0.4 + cosTheta) * center;
	
    //gl_FragColor = texture( mainText, UV) * (normal.y + 0.5) * height;
	
	//gl_FragColor = texture(mainText, UV) * (0.4 + cosTheta) * height;
}
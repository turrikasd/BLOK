#version 150

void main(){
	//gl_FragColor = gl_FragCoord.zzzz;//vec4(1.0, gl_FragCoord.zz, 1.0);
	gl_FragDepth = gl_FragCoord.z;
}
#version 410 

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec4 tangent;
layout(location=3) in vec2 TexCoord1;
layout(location=4) in vec2 TexCoord2;

out vec4 vPos;
out vec3 vNorm;
out vec3 vTan;
out vec3 vbiTan;
out vec2 vUV1;
out vec2 vUV2;

uniform mat4 ProjectionView;

void main() {

	vPos = Position;
	vNorm = Normal.xyz;
	vTan = tangent.xyz;
	vbiTan = cross(vNorm, vTan);
	vUV1 = TexCoord1;
	vUV2 = TexCoord2;
	
	gl_Position= ProjectionView * Position;

}
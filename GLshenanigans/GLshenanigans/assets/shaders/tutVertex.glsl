#version 410 

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec2 TexCoord;

out vec4 vPos;
out vec4 vNorm;
out vec2 vUV;

uniform mat4 ProjectionView;

void main() {

	vPos = Position;
	vNorm = Normal;
	vUV = TexCoord;
	
	gl_Position= ProjectionView * Position;

}
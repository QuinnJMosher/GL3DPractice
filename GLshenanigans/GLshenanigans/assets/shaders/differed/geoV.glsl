#version 410 

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
//layout(location=3) in vec2 TexCoord;

//out vec2 vTexCoord;

out vPosition;
out vNormal;

uniform mat4 View;
uniform mat4 ProjectionView;

void main() {

	//vTexCoord = TexCoord;
	vPosition = View * Position;
	vNormal = normalize(View * Position);
	
	gl_Position= ProjectionView * Position;

}
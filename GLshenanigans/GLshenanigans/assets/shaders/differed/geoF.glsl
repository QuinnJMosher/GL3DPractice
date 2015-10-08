#version 410

//in vec2 vTexCoord;

in vec4 vPosition;
in vec4 vNormal;

layout(location = 0) out vec3 gpassAlbedo;
layout(location = 1) out vec3 gpassPosition;
layout(location = 2) out vec3 gpassNormal;

//uniform sampler2D diffuse;

void main() {

	//FragColor = texture(diffuse,vTexCoord);
	gpassAlbedo = vec3(1);
	
	gpassPosition = vPosition.xyz;
	gpassNormal = vNormal.xyz;
	
}
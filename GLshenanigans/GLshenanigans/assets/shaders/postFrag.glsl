#version 410

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D diffuse;

vec4 Simple() {
	return texture(diffuse,vTexCoord);
}



void main() {

	FragColor = Simple();
	
}
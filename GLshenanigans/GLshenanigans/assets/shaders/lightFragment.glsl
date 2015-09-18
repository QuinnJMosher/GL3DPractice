#version 410

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D diffuse;

uniform vec3 ambientCol = vec3(0.5f, 0.5f, 0.5f);

uniform vec3 directLightDir = vec3(0.0f, -1.0f, 0.0f);
uniform vec3 directLightCol = vec3(0.6f, 0.6f, 0.6f);

void main() {

	FragColor = texture(diffuse,vTexCoord);
	
}
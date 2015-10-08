#version 410

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D albedoTexture;
uniform sampler2D lightTexture;

void main() {

	light = texture(lightTexture, vTexCoord).rgb;
	albedo = texture(albedoTexture, vTexCoord).rgb;
	
	FragColor = vec4(albedo * light, 1);
	
}
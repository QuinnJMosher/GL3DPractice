#version 410

in vec4 vPos;
in vec4 vNorm;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D diffuseMap;
uniform vec3 cameraPos;

uniform float specPow;

uniform vec3 lightDirection = vec3(-1, -1, -0.5f);
uniform vec3 lightColor = vec3(0.8f, 0.8f, 0.8f);

uniform vec3 ambientColor = vec3(0.5f, 0.5f, 0.5f);

void main() {
	
	vec3 color = texture(diffuseMap, vUV).xyz;
	vec3 ambient = ambientColor * color;
	
	float lambert = max(0.0f, dot(vNorm.xyz, -lightDirection));
	vec3 diffuse = lightColor * lambert * color;
	
	FragColor = vec4(ambient + diffuse, 1);
	
}
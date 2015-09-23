#version 410

in vec4 vPos;
in vec4 vNorm;
in vec4 vTan;
in vec4 vbiTan;
in vec2 vUV1;
in vec2 vUV2;

out vec4 FragColor;

uniform sampler2D diffuseMap;

uniform vec3 cameraPos;

uniform float specPow = 32.0f;

uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform vec3 ambientColor;

void main()
{
	vec3 lightDirNorm = normalize(lightDirection);
	
	vec3 color = texture(diffuseMap, vUV1).xyz;
	vec3 ambient = ambientColor * color;
	
	float lambert = max(0.0f, dot(vNorm.xyz, -lightDirNorm));
	vec3 diffuse = lightColor * lambert * color;
	
	vec3 eye = normalize(cameraPos - vPos.xyz);
	vec3 R  = normalize(reflect(lightDirNorm, vNorm.xyz));
	
	float specTerm = pow(max(0.0f, dot(R,eye)), specPow);
	
	vec3 specular = lightColor * specTerm * color;
	
	specular = clamp(specular,0.0f,1.0f);
	
	FragColor = vec4(specular + diffuse + ambient,1);	
}
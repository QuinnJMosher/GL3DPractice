#version 410

in vec4 vPos;
in vec4 vNorm;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D diffuseMap;


uniform vec3 cameraPos;



uniform float specPow = 32.0f;

uniform vec3 lightDirection = vec3(-1, -1, -0.5f);
uniform vec3 lightColor = vec3(0.6f, 0.6f, 0.6f);

uniform vec3 ambientColor = vec3(0.5f, 0.5f, 0.5f);

void main()
{
	vec3 lightDirNorm = normalize(lightDirection);
	
	vec3 color = texture(diffuseMap, vUV).xyz;
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
#version 410

in vec4 vPos;
in vec3 vNorm;
in vec3 vTan;
in vec3 vbiTan;
in vec2 vUV1;
in vec2 vUV2;

out vec4 FragColor;

uniform sampler2D diffuseMap;
uniform sampler2D normal;

uniform vec3 cameraPos;

uniform float specPow = 32.0f;

uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform vec3 ambientColor;

void main()
{
	mat3 TBN = mat3(normalize(vTan),
					normalize(vbiTan),
					normalize(vNorm));
	vec3 N = texture(normal, vUV1).xyz * 2 - 1;
	vec3 fNorm = normalize(TBN * N);

	vec3 lightDirNorm = normalize(lightDirection);
	
	vec3 color = texture(diffuseMap, vUV1).xyz;
	vec3 ambient = ambientColor * color;
	
	float lambert = max(0.0f, dot(fNorm.xyz, -lightDirNorm));
	vec3 diffuse = lightColor * lambert * color;
	
	vec3 eye = normalize(cameraPos - vPos.xyz);
	vec3 R  = normalize(reflect(lightDirNorm, fNorm.xyz));
	
	float specTerm = pow(max(0.0f, dot(R,eye)), specPow);
	
	vec3 specular = lightColor * specTerm * color;
	
	specular = clamp(specular,0.0f,1.0f);
	
	FragColor = vec4(specular + diffuse + ambient,1);	
	//FragColor = vec4(specular + diffuse,1);
}
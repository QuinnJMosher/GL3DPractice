#version 410

in vec4 vPos;
in vec4 vNorm;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D diffuseMap;
uniform vec3 cameraPos;

uniform vec3 L = vec3(-1, -1, -0.5f);

uniform vec3 kA = vec3(1,0,0);
uniform vec3 kD = vec3(1,0,0);
uniform vec3 kS = vec3(1,0,0);

uniform vec3 iA = vec3(0.25f, 0.25f, 0.25f);
uniform vec3 iD = vec3(1, 1, 1);
uniform vec3 iS = vec3(1, 1, 1);
uniform float specPow = 32.0f;

void main() {
	
	vec3 color = texture(diffuseMap, vUV).xyz;
	
	vec3 Ambient = kA * iA;

	float Ndl = max(0.0f, dot(vNorm, -L));
	Vec3 Diffuse = kD * iD * Ndl;
	
	float R = reflect(L, vNorm);
	vec3 E = normalize(cameraPos - vPos);
	
	float specTerm = pow( min(0.0f, dot(R, E)), specPow);
	vec3 Specular = kS * iS * specTerm;
	
	FragColor = vec4(Ambient + Diffuse + Specular, 1);
	
}
#version 410

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D Target;

vec4 Simple() {
	return texture(Target, fTexCoord);
}

void main() {
	FragColor = Simple();
}
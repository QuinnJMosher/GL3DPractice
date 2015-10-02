#version 410

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D diffuse;

vec4 Simple() {
	return texture(diffuse,vTexCoord);
}

vec4 BoxBlur() {
	vec2 texel = 1.0f / textureSize(diffuse, 0).xy;
	
	vec4 color = texture(diffuse, vTexCoord);
	color += texture(diffuse, vTexCoord + vec2(-texel.x,  texel.y));
	color += texture(diffuse, vTexCoord + vec2(-texel.x,   	    0));
	color += texture(diffuse, vTexCoord + vec2(-texel.x, -texel.y));
	color += texture(diffuse, vTexCoord + vec2(		  0,  texel.y));
	color += texture(diffuse, vTexCoord + vec2(		  0, -texel.y));
	color += texture(diffuse, vTexCoord + vec2( texel.x,  texel.y));
	color += texture(diffuse, vTexCoord + vec2( texel.x, 		0));
	color += texture(diffuse, vTexCoord + vec2( texel.x, -texel.y));
	
	return color / 9;
}

vec4 Distort() {
	vec2 mid = vec2(0.5f);
	
	float distanceFromCentre = distance(vTexCoord, mid);
	vec2 normalizedCoord = normalize(vTexCoord - mid);
	float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.02f;
	
	vec2 newCoord = mid + bias * normalizedCoord;
	return texture(diffuse, newCoord);
}

vec4 GrayScale() {
	vec4 basecolor = texture(diffuse, vTexCoord);
	float avgColor = (basecolor.x + basecolor.y + basecolor.z) / 3;
	return vec4(avgColor, avgColor, avgColor, 1);
}

vec4 ScanLines() {
	vec2 texel = 1.0f / textureSize(diffuse, 0).xy;
	
	vec4 color = texture(diffuse, vTexCoord);
	
	int yPos = int(textureSize(diffuse, 0).y * vTexCoord.y);
	if (yPos % 3 == 0) {
		color = (color + vec4(0, 0.5f, 0, 1)) / 2;
	}
	
	return color;
	
}

vec4 Sharpen() {
	vec2 texel = 1.0f / textureSize(diffuse, 0);
	
	vec4 color = texture(diffuse, vTexCoord) * (11.0f / 3.0f);
	color += texture(diffuse, vTexCoord + vec2(0, texel.y)) * (-2.0f / 3.0f);
	color += texture(diffuse, vTexCoord - vec2(0, texel.y)) * (-2.0f / 3.0f);
	color += texture(diffuse, vTexCoord + vec2(texel.x, 0)) * (-2.0f / 3.0f);
	color += texture(diffuse, vTexCoord - vec2(texel.x, 0)) * (-2.0f / 3.0f);
	color.w = 1.0f;
	
	return color;
}

vec4 Sobel() {
	vec4 colorX = vec4(0, 0, 0, 1);
	vec4 colorY = vec4(0, 0, 0, 1);
	
	vec2 texel = 1.0f / textureSize(diffuse, 0);
	
	//x
	colorX += texture(diffuse, vTexCoord + vec2(-texel.x, -texel.y)) * -1;
	colorX += texture(diffuse, vTexCoord + vec2(-texel.x, 0)) * -2;
	colorX += texture(diffuse, vTexCoord + vec2(-texel.x, texel.y)) * -1;
	
	colorX += texture(diffuse, vTexCoord + vec2(texel.x, -texel.y)) * 1;
	colorX += texture(diffuse, vTexCoord + vec2(texel.x, 0)) * 2;
	colorX += texture(diffuse, vTexCoord + vec2(texel.x, texel.y)) * 1;
	
	//y
	colorY += texture(diffuse, vTexCoord + vec2(-texel.x, texel.y)) * -1;
	colorY += texture(diffuse, vTexCoord + vec2(0, texel.y)) * -2;
	colorY += texture(diffuse, vTexCoord + vec2(texel.x, texel.y)) * -1;
	
	colorY += texture(diffuse, vTexCoord + vec2(-texel.x, -texel.y)) * 1;
	colorY += texture(diffuse, vTexCoord + vec2(0, -texel.y)) * 2;
	colorY += texture(diffuse, vTexCoord + vec2(texel.x, -texel.y)) * 1;
	
	vec4 totalColor = vec4(0, 0, 0, 1);
	
	totalColor.x = sqrt(pow(colorX.x, 2) + pow(colorY.x, 2));
	totalColor.y = sqrt(pow(colorX.y, 2) + pow(colorY.y, 2));
	totalColor.z = sqrt(pow(colorX.z, 2) + pow(colorY.z, 2));
	
	return totalColor;
}

void main() {
	FragColor = Sobel();
	
}
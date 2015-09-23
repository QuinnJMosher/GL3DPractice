#ifndef _GLstructs_h_
#define _GLstructs_h_

#include "glm/glm.hpp"
#include <glm/ext.hpp>
using glm::vec4;
using glm::vec3;

struct Vertex
{
	vec4 position;
	vec4 color;
};

struct VertexUV
{
	vec4 position;
	vec4 normal;
	vec4 tangent;
	vec4 biTangent;
	glm::vec2 uv1;
	glm::vec2 uv2;
};

//RenderObject
struct GLdata {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	unsigned int indexCount;
};

struct Texture {
	int imageWidth;
	int imageHeight;
	int imageFormat;
	unsigned int textureID;
};

struct DirectionLight {
	vec3 direction;
	vec3 color;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float falloff;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float falloff;
};

#endif
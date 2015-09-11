#ifndef _GLstructs_h_
#define _GLstructs_h_

#include "glm/glm.hpp"
#include <glm/ext.hpp>
using glm::vec4;

struct Vertex
{
	vec4 position;
	vec4 color;
};

struct VertexUV
{
	vec4 position;
	glm::vec2 uv;
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

#endif
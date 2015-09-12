#ifndef _quickGLfuncs_h_
#define _quickGLfuncs_h_

#include "GLstructs.h"
#include "gl_core_4_4.h"
#include "tiny_obj_loader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using glm::mat4;
typedef unsigned int programID;

struct Geometry {
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<GLdata> glInfo;
};

namespace QuickFunc {

	GLdata GenerateGrid(unsigned int rows, unsigned int cols);
	programID QuickRenderProg();
	void EasyReder(programID renderProgram, mat4 projViewMat, GLdata rednerData, float time);

	Geometry* loadGeometry(std::string in_filename);
	void renderGeo(programID renderProgram, mat4 projViewMat, Geometry* in_target);

	GLdata createDumbBox();

	programID QuickTextProg();

	Texture* LoadTexture(std::string in_fileName);
	void renderTex(programID renderProgram, mat4 projViewMat, GLdata in_target, Texture* in_texture);

	GLdata LoadFBX(std::string in_filename);
	Texture* LoadFBXTexture(std::string in_filename);

	unsigned int loadShader(unsigned int type, const char* fileName);
	programID makeProgram(const char* vertexShFileName, const char* fragmentShFileName);
}

#endif
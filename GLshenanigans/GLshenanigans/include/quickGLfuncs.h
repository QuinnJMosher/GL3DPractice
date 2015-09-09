#ifndef _quickGLfuncs_h_
#define _quickGLfuncs_h_

#include "GLstructs.h"
#include "gl_core_4_4.h"
#include <iostream>

using glm::mat4;
typedef unsigned int programID;

namespace QuickFunc {

	GLdata GenerateGrid(unsigned int rows, unsigned int cols);
	programID QuickRenderProg();
	void EasyReder(programID renderProgram, mat4 projViewMat, GLdata rednerData, float time);
}

#endif

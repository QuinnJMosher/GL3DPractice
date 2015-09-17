#ifndef _Renderer_h_
#define _Renderer_h_

#include "Model.h"
#include <string>

class FBXVertex;

namespace Renderer {
	namespace Drawing {
		void DrModel(Model* in_target, glm::mat4 in_projViewMat);
		void DrGLdata(GLdata* in_target, glm::mat4 in_projViewMat);
		void DrGldataWithTex(GLdata* in_target, Texture* in_texture, glm::mat4 in_projViewMat);
	}

	namespace Memory {
		GLdata* CreateGLdata(FBXVertex* in_verticies, unsigned int in_vertexCT, 
							unsigned int* in_indicies, unsigned int in_indexCT);
		Texture* CreateTextData(std::string in_filename);
		Texture* CreateTexture(unsigned char* in_data, int in_imgWidth, int in_imgHeight, int in_format = -1);

		unsigned int CreateDefaultProgram();
		static unsigned int DefaultProgram;
		unsigned int CreateProgram(const char* in_vertexShaderFile, const char* in_fragmentShaderFile);
		unsigned int CreateShader(unsigned int in_type, const char* in_filepath);

		void DeleteGLdata(GLdata* in_target);
		void DeleteTexture(Texture* in_target);
		void DeleteProgram(unsigned int in_target);
	}
}


#endif
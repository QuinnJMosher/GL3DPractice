#ifndef _Renderer_h_
#define _Renderer_h_

#include "Model.h"

class FBXVertex;
class string;

namespace Renderer {
	namespace Drawing {
		void DrModel(Model* in_target);
		void DrModelWireframe(Model* in_target);
		void DrGLdata(GLdata* in_target);
		void DrGldataWithTex(GLdata* in_target, Texture* in_texture);
	}

	namespace Memory {
		GLdata* CreateGLdata(FBXVertex* in_verticies, unsigned int in_vertexCT, 
							unsigned int* in_indicies, unsigned int in_indexCT);
		Texture* CreateTextData(string in_filename);
		Texture* CreateTexture(unsigned char* in_data, int in_imgWidth, int in_imgHeight, int in_format = -1);

		unsigned int CreateDefaultProgram();
		static unsigned int DefaultProgram;
		unsigned int CreateProgram(const char* in_vertexShaderFile, const char* in_fragmentShaderFile);

		void DeleteGLdata(GLdata* in_target);
		void DeleteTexture(Texture* in_target);
		void DeleteProgram(unsigned int in_target);
	}
}


#endif
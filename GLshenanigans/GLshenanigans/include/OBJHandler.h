#ifndef _OBJHandler_h_
#define _OBJHandler_h_

#include <string>
using std::string;
#include <vector>
using std::vector;
#include "tiny_obj_loader.h"
#include "gl_core_4_4.h"

#include "Camera.h"

class Geometry {
public:
	static void ReadyShaderProgram();
	static void SetMainCamera(Camera* in_mainCamera);
	static Geometry* Load(const string in_filename);
	void Render();
private:
	Geometry();
	void createGLBuffers();

	static Camera* mainCamera;
	static unsigned int shaderProgram;

	struct OGLInfo
	{
		unsigned int VAO;
		unsigned int VBO;
		unsigned int IBO;
		unsigned int indexCount;
	};

	vector<OGLInfo> shapeInfo;
	string filename;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
};

#endif
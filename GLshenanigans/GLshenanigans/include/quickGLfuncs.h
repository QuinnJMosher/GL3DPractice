#ifndef _quickGLfuncs_h_
#define _quickGLfuncs_h_

#include "GLstructs.h"
#include "gl_core_4_4.h"
#include "tiny_obj_loader.h"
#include "Camera.h"
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
	void renderTex(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture);

	GLdata LoadFBX(std::string in_filename);
	Texture* LoadFBXTexture(std::string in_filename, int in_index = 0);

	unsigned int loadShader(unsigned int type, const char* fileName);
	programID makeProgram(const char* vertexShFileName, const char* fragmentShFileName);

	void renderWithLight(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture, DirectionLight in_light);
	void renderWithPointLight(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture, PointLight in_light);
	void renderWithSpotLight(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture, SpotLight in_light);
	static vec3 ambientLightColor = vec3(0.5f, 0.5f, 0.5f);

	void renderNormal(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture, Texture* in_normalMap, DirectionLight in_light);
	
	FrameBuffer createFrameBuffer(int in_Width, int in_heignt);
	void drawToBuffer(programID in_renderProgram, Camera in_camera, GLdata in_model, Texture* in_texture, Texture* in_normalMap, DirectionLight in_light, FrameBuffer in_targetBuffer);
	void drawBuffer(programID in_renderProgram, Camera in_camera, GLdata in_model, FrameBuffer in_sorceBuffer);
	void clearFrameBuffer(FrameBuffer in_framebuffer);

	GLdata ReadyPostProcessing(int in_screenWidth, int in_screenHeight);
	void DrawPostProcessing(FrameBuffer in_frambuffer, GLdata in_targetBox, programID in_program);
}

#endif
#include "Model.h"//header
//addit
#include "Renderer.h"
#include <iostream>
#include <vector>
//using std::string;
#include "glm\ext.hpp"
//using glm::vec4;
//using glm::mat4;
#include "FBXFile.h"
#include "stb\stb_image.h"
#include "tiny_obj_loader.h"
#include "GLstructs.h"

using namespace Renderer;

FBXFile* Model::FBXInstance = nullptr;
unsigned int Model::defaultProgram = 0;

Model::Model() {
	if (FBXInstance == nullptr) {
		std::cerr << "Warning: Model Class has auto initalized";
		Startup();
	}

	renderObject = new GLdata();
	fileName = new std::string("");
	fType = Mod_FileType::Mod_None;
	currentTexture = nullptr;
	textureName = new std::string("");

	position = new vec4(0, 0, 0, 1);
	rotation = new vec4(0, 0, 0, 1);

	matIsDirty = true;
	transform = new glm::mat4();
}

Model::~Model() {
	//have renderer delete gl stuff
	Memory::DeleteGLdata(renderObject);
	Memory::DeleteTexture(currentTexture);
	Memory::DeleteProgram(personalProgram);
	delete fileName;
	delete textureName;
	delete position;
	delete rotation;
	delete transform;
}

void Model::Startup() {
	FBXInstance = new FBXFile();
	defaultProgram = Memory::CreateDefaultProgram();
}
void Model::Shutdown() {
	delete FBXInstance;
}

Model* Model::LoadOBJ(std::string in_fileName) {
	Model* newModel = new Model();
	std::vector<tinyobj::shape_t> shapes = std::vector<tinyobj::shape_t>();
	std::vector<tinyobj::material_t> mats = std::vector<tinyobj::material_t>();

	std::string err = tinyobj::LoadObj(shapes, mats, in_fileName.c_str());
	printf(err.c_str());


	for (int i = 0; i < shapes.size(); i++) {
		for (int j = 0; j < shapes[i].mesh.positions.size(); j++) {

		}
	}

	return new Model();
}
Model* Model::LoadFBX(std::string in_fileName, int in_modelIndex = 0, int in_textureIndex = -1) {
	//pull in using fbx
	//clean up after yourself!
	//try not unloading and compareing paths to limit unnessisary re-loading
	return new Model();
}
void Model::DeleteFile(Model* in_target) {
	delete in_target;
}

void Model::LoadTexture(std::string in_fileName, int in_FBXIndex = -1) {
	if (in_FBXIndex == -1) {
		//fbx style
		//fbx can gl-ify the textures for me!
	}
	else
	{
		//stb style
	}
}
void Model::GiveProgram(unsigned int in_programID) {
	personalProgram = in_programID;
}
void Model::GiveProgram(std::string in_VertexShaderFile, std::string in_fragShaderFile) {
	personalProgram = Memory::CreateProgram(in_VertexShaderFile.c_str(), in_fragShaderFile.c_str());
}

void Model::SetPostion(float in_x, float in_y, float in_z) {
	position->x = in_x;
	position->y = in_y;
	position->z = in_z;
	matIsDirty = true;
}

void Model::Translate(float in_x, float in_y, float in_z) {
	position->x += in_x;
	position->y += in_y;
	position->z += in_z;
	matIsDirty = true;
}

void Model::SetRotation(float in_x, float in_y, float in_z) {
	rotation->x = in_x;
	rotation->y = in_y;
	rotation->z = in_z;
	matIsDirty = true;
}

void Model::Rotate(float in_x, float in_y, float in_z) {
	rotation->x += in_x;
	rotation->y += in_y;
	rotation->z += in_z;
	matIsDirty = true;
}

glm::mat4* Model::GetTransoform() {
	if (matIsDirty) {
		//apply rotaion
		//apply position
		matIsDirty = false;
	}
	return transform;
}

GLdata* Model::GetGLdata()
{
	return renderObject;
}

Texture* Model::GetTexture() {
	return currentTexture;
}

unsigned int Model::GetProgram() {
	if (personalProgram == 0) {
		return defaultProgram;
	}
	else 
	{
		return personalProgram;
	}
}
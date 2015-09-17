#include "Model.h"//header
//addit
#include <iostream>
#include <string>
using std::string;
#include "glm\glm.hpp"
#include "glm\ext.hpp"
using glm::vec4;
using glm::mat4;
#include "FBXFile.h"
#include "stb\stb_image.h"
#include "tiny_obj_loader.h"
#include "GLstructs.h"

FBXFile* Model::FBXInstance = nullptr;
unsigned int Model::defaultProgram = 0;

Model::Model() {
	if (FBXInstance == nullptr) {
		std::cerr << "Warning: Model Class has auto initalized";
		Startup();
	}

	renderObject = new GLdata();
	fileName = new string("");
	fType = Mod_FileType::Mod_None;
	currentTexture = nullptr;
	textureName = new string("");

	position = new vec4(0, 0, 0, 1);
	rotation = new vec4(0, 0, 0, 1);

	matIsDirty = true;
	transform = new mat4();
}

Model::~Model() {
	delete renderObject;
	delete fileName;
	delete currentTexture;
	delete textureName;
	delete position;
	delete rotation;
	delete transform;
}

void Model::Startup() {
	FBXInstance = new FBXFile();
	//get a default program from renderer here
}
void Model::Shutdown() {
	delete FBXInstance;
}

Model* Model::LoadOBJ(string in_fileName) {
	//pull in using tinyObj
	return new Model();
}
Model* Model::LoadFBX(string in_fileName, int in_modelIndex = 0, int in_textureIndex = -1) {
	//pull in using fbx
	//clean up after yourself!
	return new Model();
}
void Model::DeleteFile(Model* in_target) {
	delete in_target;
}

void Model::LoadTexture(string in_fileName, int in_FBXIndex = -1) {
	if (in_FBXIndex == -1) {
		//fbx style
	}
	else
	{
		//stb style
	}
}
void Model::GiveProgram(unsigned int in_programID) {
	personalProgram = in_programID;
}
void Model::GiveProgram(string in_VertexShaderFile, string in_fragShaderFile) {
	//create shaders from file and have renderer handle it
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

mat4* Model::getTransoform() {
	if (matIsDirty) {
		//apply rotation
		//apply position
		matIsDirty = false;
	}
	return transform;
}
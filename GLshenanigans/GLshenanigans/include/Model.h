#ifndef _Model_h_
#define _Model_h_

#include "glm\glm.hpp"
#include <string>

class vec4;
class FBXFile;
struct GLdata;
struct Texture;

enum Mod_FileType {
	Mod_None,
	Mod_OBJ,
	Mod_FBX
};

class Model {
public:
	//loading/management
	static void Startup();
	static void Shutdown();

	static Model* LoadOBJ(std::string in_fileName);
	static Model* LoadFBX(std::string in_fileName, int in_modelIndex = 0, int in_textureIndex = -1);
	static void DeleteFile(Model* in_target);
	
	void LoadTexture(std::string in_fileName, int in_FBXIndex = -1);
	void GiveProgram(unsigned int in_programID);
	void GiveProgram(std::string in_VertexShaderFile, std::string in_fragShaderFile);

	//position
	void SetPostion(float in_x, float in_y, float in_z);
	void Translate(float in_x, float in_y, float in_z);

	void SetRotation(float in_x, float in_y, float in_z);
	void Rotate(float in_x, float in_y, float in_z);

	glm::mat4* GetTransoform();
	GLdata* GetGLdata();
	Texture* GetTexture();
	unsigned int GetProgram();

private:
	Model();
	~Model();

	static FBXFile* FBXInstance;

	static unsigned int defaultProgram;
	unsigned int personalProgram;

	GLdata* renderObject;
	std::string* fileName;
	Mod_FileType fType;
	Texture* currentTexture;
	std::string* textureName;

	vec4* position;
	vec4* rotation;

	bool matIsDirty;
	glm::mat4* transform;
};

#endif
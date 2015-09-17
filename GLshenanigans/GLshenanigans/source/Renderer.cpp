#include "Renderer.h"//header
//addit
//#include "GLFW\glfw3.h"
#include <string>
using std::string;
#include "gl_core_4_4.h"
#include "GLstructs.h"
#include "FBXFile.h"
#include "stb\stb_image.h"

void Renderer::Drawing::DrModel(Model* in_target);
void Renderer::Drawing::DrModelWireframe(Model* in_target);
void Renderer::Drawing::DrGLdata(GLdata* in_target);
void Renderer::Drawing::DrGldataWithTex(GLdata* in_target, Texture* in_texture);

unsigned int Renderer::Memory::DefaultProgram = 0;

GLdata* Renderer::Memory::CreateGLdata(FBXVertex* in_verticies, unsigned int in_vertexCT, unsigned int* in_indicies, unsigned int in_indexCT) {
	GLdata* output = new GLdata();
	output->indexCount = in_indexCT;

	glGenVertexArrays(1, &output->VAO);
	glGenBuffers(1, &output->VBO);
	glGenBuffers(1, &output->IBO);

	glBindVertexArray(output->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, output->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, output->IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(FBXVertex), in_verticies, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, in_indexCT * sizeof(unsigned int), in_indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);
	glEnableVertexAttribArray(8);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::PositionOffset);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::ColourOffset);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::NormalOffset);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::TangentOffset);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::BiNormalOffset);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::IndicesOffset);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::WeightsOffset);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::TexCoord1Offset);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::Offsets::TexCoord2Offset);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return output;
}

Texture* Renderer::Memory::CreateTextData(string in_filename) {
	int width, height, format;
	unsigned char* data = stbi_load(in_filename.c_str, &width, &height, &format, STBI_default);

	Texture* output = Renderer::Memory::CreateTexture(data, width, height, format);

	stbi_image_free(data);

	return output;
}

Texture* Renderer::Memory::CreateTexture(unsigned char* in_data, int in_imgWidth, int in_imgHeight, int in_format = -1) {
	Texture* output = new Texture();
	output->imageWidth = in_imgWidth;
	output->imageHeight = in_imgHeight;
	output->imageFormat = in_format;

	glGenTextures(1, &(output->textureID));
	glBindTexture(GL_TEXTURE_2D, output->textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, output->imageWidth, output->imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, in_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return output;
}

unsigned int Renderer::Memory::CreateDefaultProgram() {
	const char* vertexSource = "#version 410\n \
								layout(location=0) in vec4 Position; \
								layout(location=1) in vec4 Color; \
								layout(location=2) in vec4 Normal; \
								layout(location=3) in vec4 Tangent; \
								layout(location=4) in vec4 BiNormal; \
								layout(location=5) in vec4 Indices; \
								layout(location=6) in vec4 Weights; \
								layout(location=7) in vec2 TexCoord1; \
								layout(location=8) in vec2 TexCoord2; \
								out vec2 vTexCoord; \
								uniform mat4 ProjectionView; \
								void main() { \
								vTexCoord = TexCoord1; \
								gl_Position= ProjectionView * Position; \
								}";
	const char* fragmentSource = "#version 410\n \
								in vec2 vTexCoord; \
								out vec4 FragColor; \
								uniform sampler2D diffuse; \
								void main() { \
								FragColor = texture(diffuse,vTexCoord); \
								}";
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vertexSource, 0);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fragmentSource, 0);
	glCompileShader(fragmentShader);

	unsigned int newProgram = glCreateProgram();
	glAttachShader(newProgram, vertexShader);
	glAttachShader(newProgram, fragmentShader);
	glLinkProgram(newProgram);

	int success = GL_FALSE;
	glGetProgramiv(newProgram, GL_LINK_STATUS, &success);
	glGetProgramiv(newProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(newProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(newProgram, infoLogLength, 0, infoLog);
		printf("Error: failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return newProgram;
}

unsigned int Renderer::Memory::CreateProgram(const char* in_vertexShaderFile, const char* in_fragmentShaderFile);

void Renderer::Memory::DeleteGLdata(GLdata* in_target);
void Renderer::Memory::DeleteTexture(Texture* in_target);
void Renderer::Memory::DeleteProgram(unsigned int in_target);
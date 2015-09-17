#include "Renderer.h"//header

void Renderer::Drawing::DrModel(Model* in_target);
void Renderer::Drawing::DrModelWireframe(Model* in_target);
void Renderer::Drawing::DrGLdata(GLdata* in_target);
void Renderer::Drawing::DrGldataWithTex(GLdata* in_target, Texture* in_texture);

unsigned int Renderer::Memory::DefaultProgram = 0;

GLdata* Renderer::Memory::CreateGLdata(FBXVertex* in_verticies, unsigned int in_vertexCT, unsigned int* in_indicies, unsigned int in_indexCT) {

}
Texture* Renderer::Memory::CreateTextData(string in_filename);
Texture* Renderer::Memory::CreateTexture(unsigned char* in_data, int in_imgWidth, int in_imgHeight);

unsigned int Renderer::Memory::CreateDefaultProgram();
unsigned int Renderer::Memory::CreateProgram(const char* in_vertexShaderFile, const char* in_fragmentShaderFile);

void Renderer::Memory::DeleteGLdata(GLdata* in_target);
void Renderer::Memory::DeleteTexture(Texture* in_target);
void Renderer::Memory::DeleteProgram(unsigned int in_target);
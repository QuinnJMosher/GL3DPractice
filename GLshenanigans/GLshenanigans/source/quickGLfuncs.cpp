#pragma warning(disable : 4996)
#include "quickGLfuncs.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"
#include "FBXFile.h"

GLdata QuickFunc::GenerateGrid(unsigned int rows, unsigned int cols) {

	//generate positions
	GLdata dataOut;
	VertexUV* aoVertices = new VertexUV[rows * cols];
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			aoVertices[r * cols + c].position = vec4(
													(float)c, 0, (float)r, 1);

			//glm::vec3 color = glm::vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
			//aoVertices[r * cols + c].color = vec4(color, 1);
			aoVertices[r * cols + c].uv1 = glm::vec2(1 - (float)r / (rows - 1),
													(float)c / (cols - 1));
		}
	}

	//generate indices
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); r++) {
		for (unsigned int c = 0; c < (cols - 1); c++) {

			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}

	

	dataOut.indexCount = (rows - 1) * (cols - 1) * 6;

	//generate buffers
	glGenBuffers(1, &(dataOut.VBO));
	glGenBuffers(1, &(dataOut.IBO));
	glGenVertexArrays(1, &(dataOut.VAO));

	glBindVertexArray(dataOut.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, dataOut.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dataOut.IBO);
	
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(VertexUV), aoVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataOut.indexCount * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexUV), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)(sizeof(glm::vec4)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] aoVertices;
	delete[] auiIndices;
	return dataOut;
}

programID QuickFunc::QuickRenderProg() {
	//hardcoded shaders
	const char* vertexShaderSrc = "#version 410\n \
								layout(location=0) in vec4 Position; \
								layout(location=1) in vec4 Colour; \
								out vec4 vColour; \
								uniform mat4 ProjectionView; \
								void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";

	const char* vertexShaderSineWave = "#version 410\n \
									in vec4 Position; \
									in vec4 Colour; \
									out vec4 vColour; \
									uniform mat4 ProjectionView; \
									uniform float time;\
									uniform float heightScale;\
									void main() { \
										vColour = Colour; \
										vec4 P = Position; \
										P.y += sin(time + Position.x) * heightScale;\
										gl_Position = ProjectionView * P; \
									}";

	const char* fragmentShaderSrc = "#version 410\n \
									in vec4 vColour; \
									out vec4 FragColor; \
									void main() { FragColor = vColour; } ";

	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vertexShaderSrc, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fragmentShaderSrc, 0);
	glCompileShader(fragmentShader);

	programID newProgramID = glCreateProgram();
	glAttachShader(newProgramID, vertexShader);
	glAttachShader(newProgramID, fragmentShader);
	glLinkProgram(newProgramID);

	glGetProgramiv(newProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(newProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(newProgramID, infoLogLength, 0, infoLog);
		printf("Error: failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return newProgramID;
}

void QuickFunc::EasyReder(programID renderProgram, mat4 projViewMat, GLdata rednerData, float time) {
	glUseProgram(renderProgram);

	unsigned int projectionViewuniform = glGetUniformLocation(renderProgram, "ProjectionView");
	unsigned int timeUniform = glGetUniformLocation(renderProgram, "time");
	unsigned int heigntScaleUniform = glGetUniformLocation(renderProgram, "heightScale");

	glUniformMatrix4fv(projectionViewuniform, 1, false, glm::value_ptr(projViewMat));
	//glUniform1f(timeUniform, time);
	//glUniform1f(heigntScaleUniform, 0);

	glBindVertexArray(rednerData.VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, rednerData.indexCount, GL_UNSIGNED_INT, 0);
}

Geometry* QuickFunc::loadGeometry(std::string in_filename) {
	Geometry* output = new Geometry();
	output->shapes = std::vector<tinyobj::shape_t>();
	output->materials = std::vector<tinyobj::material_t>();
	output->glInfo = std::vector<GLdata>();

	std::string err = tinyobj::LoadObj(output->shapes, output->materials, in_filename.c_str());
	printf(err.c_str());

	output->glInfo.resize(output->shapes.size());

	for (unsigned int mesh_index = 0; mesh_index < output->shapes.size(); mesh_index++) {
		glGenVertexArrays(1, &(output->glInfo[mesh_index].VAO));
		glGenBuffers(1, &(output->glInfo[mesh_index].VBO));
		glGenBuffers(1, &(output->glInfo[mesh_index].IBO));

		glBindVertexArray(output->glInfo[mesh_index].VAO);

		unsigned int floatCount = output->shapes[mesh_index].mesh.positions.size();
		floatCount += output->shapes[mesh_index].mesh.normals.size();
		floatCount += output->shapes[mesh_index].mesh.texcoords.size();

		std::vector<float> vertexData;
		vertexData.reserve(floatCount);

		vertexData.insert(vertexData.end(),
							output->shapes[mesh_index].mesh.positions.begin(),
							output->shapes[mesh_index].mesh.positions.end());

		vertexData.insert(vertexData.end(),
							output->shapes[mesh_index].mesh.normals.begin(),
							output->shapes[mesh_index].mesh.normals.end());

		output->glInfo[mesh_index].indexCount =
							output->shapes[mesh_index].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, output->glInfo[mesh_index].VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, output->glInfo[mesh_index].IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			output->shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int),
			output->shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0,
					(void*)(sizeof(float) * output->shapes[mesh_index].mesh.positions.size()));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	return output;
}

void QuickFunc::renderGeo(programID renderProgram, mat4 projViewMat, Geometry* in_target) {
	glUseProgram(renderProgram);

	unsigned int projectViewUniform = glGetUniformLocation(renderProgram, "ProjectionView");

	glUniformMatrix4fv(projectViewUniform, 1, GL_FALSE, glm::value_ptr(projViewMat));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (unsigned int i = 0; i < in_target->glInfo.size(); i++) {
		glBindVertexArray(in_target->glInfo[i].VAO);
		glDrawElements(GL_TRIANGLES, in_target->glInfo[i].indexCount, GL_UNSIGNED_INT, 0);
	}

}

GLdata QuickFunc::createDumbBox() {
	float vertexData[] = {
		-5, 0, 5, 1, 0, 1,
		5, 0, 5, 1, 1, 1,
		5, 0, -5, 1, 1, 0,
		-5, 0, -5, 1, 0, 0,
	};

	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	GLdata newData;
	newData.indexCount = 6;

	glGenVertexArrays(1, &newData.VAO);
	glGenBuffers(1, &newData.VBO);
	glGenBuffers(1, &newData.IBO);

	glBindVertexArray(newData.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, newData.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newData.IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return newData;
}

programID QuickFunc::QuickTextProg() {
	const char* vertexSource = "#version 410\n \
								layout(location=0) in vec4 Position; \
								layout(location=1) in vec2 TexCoord; \
								out vec2 vTexCoord; \
								uniform mat4 ProjectionView; \
								void main() { \
								vTexCoord = TexCoord; \
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

	programID newProgram = glCreateProgram();
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

Texture* QuickFunc::LoadTexture(std::string in_fileName) {
	Texture* output = new Texture();

	unsigned char* data = stbi_load(in_fileName.c_str(), &(output->imageWidth), &(output->imageHeight), &(output->imageFormat), STBI_default);
	
	glGenTextures(1, &(output->textureID));
	glBindTexture(GL_TEXTURE_2D, output->textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, output->imageWidth, output->imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	return output;

}

void QuickFunc::renderTex(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture) {
	glUseProgram(renderProgram);

	unsigned int projectViewUniform = glGetUniformLocation(renderProgram, "ProjectionView");
	glUniformMatrix4fv(projectViewUniform, 1, GL_FALSE, glm::value_ptr(camera.getProjectionView()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_texture->textureID);

	unsigned int loc = glGetUniformLocation(renderProgram, "diffuse");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(renderProgram, "cameraPos");
	mat4 cameraTransform = camera.getWorldTransform();
	//glUniform3f(loc, cameraTransform[3][0], cameraTransform[3][1], cameraTransform[3][2]);

	glUniform3fv(loc, 1, glm::value_ptr(cameraTransform[3]));

	glBindVertexArray(in_target.VAO);
	glDrawElements(GL_TRIANGLES, in_target.indexCount, GL_UNSIGNED_INT, nullptr);
}

GLdata QuickFunc::LoadFBX(std::string in_filename) {
	//get vertex data
	FBXFile fbxFile;
	fbxFile.initialiseOpenGLTextures();
	fbxFile.load(in_filename.c_str());

	FBXMeshNode* modelData = fbxFile.getMeshByIndex(0);
	VertexUV* vertexData = new VertexUV[modelData->m_vertices.size()];

	for (int i = 0; i < modelData->m_vertices.size(); i++) {
		VertexUV newVertex;
		newVertex.position = modelData->m_vertices[i].position;
		newVertex.normal = modelData->m_vertices[i].normal;
		newVertex.tangent = modelData->m_vertices[i].tangent;
		newVertex.uv1 = modelData->m_vertices[i].texCoord1;
		newVertex.uv2 = modelData->m_vertices[i].texCoord2;
		vertexData[i] = newVertex;
	}

	//create gldata
	GLdata output;
	output.indexCount = modelData->m_indices.size();

	glGenVertexArrays(1, &output.VAO);
	glGenBuffers(1, &output.VBO);
	glGenBuffers(1, &output.IBO);

	glBindVertexArray(output.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, output.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, output.IBO);

	glBufferData(GL_ARRAY_BUFFER, modelData->m_vertices.size() * sizeof(VertexUV), vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, output.indexCount * sizeof(unsigned int), modelData->m_indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexUV), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(VertexUV), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(VertexUV), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)(sizeof(glm::vec4) * 3));
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)((sizeof(glm::vec4) * 3) + (sizeof(glm::vec2))));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//destroy stuff
	delete[] vertexData;
	fbxFile.unload();

	return output;
}

Texture* QuickFunc::LoadFBXTexture(std::string in_filename, int in_index) {
	FBXFile fbxFile;
	fbxFile.initialiseOpenGLTextures();
	fbxFile.load(in_filename.c_str());

	FBXTexture* derp = fbxFile.getTextureByIndex(in_index);

	Texture* output = new Texture();

	output->imageWidth = derp->width;
	output->imageHeight = derp->height;
	output->imageFormat = derp->format;

	unsigned char* data = derp->data;

	glGenTextures(1, &(output->textureID));
	glBindTexture(GL_TEXTURE_2D, output->textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, output->imageWidth, output->imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	fbxFile.unload();
	
	return output;
}

unsigned int QuickFunc::loadShader(unsigned int type, const char* fileName) {
	std::ifstream in(fileName);
	std::string contents((std::istreambuf_iterator<char>(in)),
						std::istreambuf_iterator<char>());
	char* src = new char[contents.length() + 1];
	strncpy(src, contents.c_str(), contents.length() + 1);
	//strncpy_s(src, (contents.length() + 1) * sizeof(char), contents.c_str(), (contents.length() + 1));
	//strncpy_s(dest, sizeInBytes, source, maxcount)
	//strncpy_s(dest[size?], src, count)
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);
	
	glCompileShader(shader);
	delete[] src;

	return shader;
}

programID QuickFunc::makeProgram(const char* vertexShFileName, const char* fragmentShFileName) {

	programID program = glCreateProgram();
	unsigned int vshader = loadShader(GL_VERTEX_SHADER, vertexShFileName);
	unsigned int fshader = loadShader(GL_FRAGMENT_SHADER, fragmentShFileName);
	int success;

	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success = GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	return program;
}

void QuickFunc::renderWithLight(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture, DirectionLight in_light) {
	glUseProgram(renderProgram);

	unsigned int projectViewUniform = glGetUniformLocation(renderProgram, "ProjectionView");
	glUniformMatrix4fv(projectViewUniform, 1, GL_FALSE, glm::value_ptr(camera.getProjectionView()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_texture->textureID);

	unsigned int loc = glGetUniformLocation(renderProgram, "diffuse");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(renderProgram, "cameraPos");
	mat4 cameraTransform = camera.getWorldTransform();
	glUniform3fv(loc, 1, glm::value_ptr(cameraTransform[3]));

	loc = glGetUniformLocation(renderProgram, "lightDirection");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.direction));

	loc = glGetUniformLocation(renderProgram, "lightColor");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.color));

	loc = glGetUniformLocation(renderProgram, "ambientColor");
	glUniform3fv(loc, 1, glm::value_ptr(ambientLightColor));

	glBindVertexArray(in_target.VAO);
	glDrawElements(GL_TRIANGLES, in_target.indexCount, GL_UNSIGNED_INT, nullptr);
}

void QuickFunc::renderWithPointLight(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture, PointLight in_light) {
	glUseProgram(renderProgram);

	unsigned int projectViewUniform = glGetUniformLocation(renderProgram, "ProjectionView");
	glUniformMatrix4fv(projectViewUniform, 1, GL_FALSE, glm::value_ptr(camera.getProjectionView()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_texture->textureID);

	unsigned int loc = glGetUniformLocation(renderProgram, "diffuse");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(renderProgram, "cameraPos");
	mat4 cameraTransform = camera.getWorldTransform();
	glUniform3fv(loc, 1, glm::value_ptr(cameraTransform[3]));

	loc = glGetUniformLocation(renderProgram, "lightPosition");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.position));

	loc = glGetUniformLocation(renderProgram, "lightColor");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.color));

	loc = glGetUniformLocation(renderProgram, "ambientColor");
	glUniform3fv(loc, 1, glm::value_ptr(ambientLightColor));

	glBindVertexArray(in_target.VAO);
	glDrawElements(GL_TRIANGLES, in_target.indexCount, GL_UNSIGNED_INT, nullptr);
}

void QuickFunc::renderWithSpotLight(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture, SpotLight in_light) {
	glUseProgram(renderProgram);

	unsigned int projectViewUniform = glGetUniformLocation(renderProgram, "ProjectionView");
	glUniformMatrix4fv(projectViewUniform, 1, GL_FALSE, glm::value_ptr(camera.getProjectionView()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_texture->textureID);

	unsigned int loc = glGetUniformLocation(renderProgram, "diffuse");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(renderProgram, "cameraPos");
	mat4 cameraTransform = camera.getWorldTransform();
	glUniform3fv(loc, 1, glm::value_ptr(cameraTransform[3]));

	loc = glGetUniformLocation(renderProgram, "lightDirection");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.direction));

	loc = glGetUniformLocation(renderProgram, "lightPosition");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.position));

	loc = glGetUniformLocation(renderProgram, "lightColor");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.color));

	loc = glGetUniformLocation(renderProgram, "ambientColor");
	glUniform3fv(loc, 1, glm::value_ptr(ambientLightColor));

	glBindVertexArray(in_target.VAO);
	glDrawElements(GL_TRIANGLES, in_target.indexCount, GL_UNSIGNED_INT, nullptr);
}

void QuickFunc::renderNormal(programID renderProgram, Camera camera, GLdata in_target, Texture* in_texture, Texture* in_normalMap, DirectionLight in_light) {
	glUseProgram(renderProgram);

	unsigned int projectViewUniform = glGetUniformLocation(renderProgram, "ProjectionView");
	glUniformMatrix4fv(projectViewUniform, 1, GL_FALSE, glm::value_ptr(camera.getProjectionView()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_texture->textureID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, in_normalMap->textureID);

	unsigned int loc = glGetUniformLocation(renderProgram, "diffuseMap");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(renderProgram, "normal");
	glUniform1i(loc, 1);

	loc = glGetUniformLocation(renderProgram, "cameraPos");
	mat4 cameraTransform = camera.getWorldTransform();
	glUniform3fv(loc, 1, glm::value_ptr(cameraTransform[3]));

	loc = glGetUniformLocation(renderProgram, "lightDirection");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.direction));

	loc = glGetUniformLocation(renderProgram, "lightColor");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.color));

	loc = glGetUniformLocation(renderProgram, "ambientColor");
	glUniform3fv(loc, 1, glm::value_ptr(ambientLightColor));

	glBindVertexArray(in_target.VAO);
	glDrawElements(GL_TRIANGLES, in_target.indexCount, GL_UNSIGNED_INT, nullptr);
}


FrameBuffer QuickFunc::createFrameBuffer(int in_Width, int in_heignt) {
	FrameBuffer output;

	output.imageWidth = in_Width;
	output.imageHeight = in_heignt;

	//generate
	glGenFramebuffers(1, &output.FBO);
	glGenTextures(1, &output.textureID);
	glGenRenderbuffers(1, &output.RenderBuffer);

	//bind
	glBindFramebuffer(GL_FRAMEBUFFER, output.FBO);
	glBindTexture(GL_TEXTURE_2D, output.textureID);
	glBindRenderbuffer(GL_RENDERBUFFER, output.RenderBuffer);

	//setup
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, in_Width, in_heignt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, output.textureID, 0);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, in_Width, in_heignt);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, output.RenderBuffer);

	//checkstatus
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("framebuffer error!\n");
	}

	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return output;
}

void QuickFunc::drawToBuffer(programID in_renderProgram, Camera in_camera, GLdata in_model, Texture* in_texture, Texture* in_normalMap, DirectionLight in_light, FrameBuffer in_targetBuffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, in_targetBuffer.FBO);
	glViewport(0, 0, in_targetBuffer.imageWidth, in_targetBuffer.imageHeight);
	
	glUseProgram(in_renderProgram);

	unsigned int projectViewUniform = glGetUniformLocation(in_renderProgram, "ProjectionView");
	glUniformMatrix4fv(projectViewUniform, 1, GL_FALSE, glm::value_ptr(in_camera.getProjectionView()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_texture->textureID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, in_normalMap->textureID);

	unsigned int loc = glGetUniformLocation(in_renderProgram, "diffuseMap");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(in_renderProgram, "normal");
	glUniform1i(loc, 1);

	loc = glGetUniformLocation(in_renderProgram, "cameraPos");
	mat4 cameraTransform = in_camera.getWorldTransform();
	glUniform3fv(loc, 1, glm::value_ptr(cameraTransform[3]));

	loc = glGetUniformLocation(in_renderProgram, "lightDirection");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.direction));

	loc = glGetUniformLocation(in_renderProgram, "lightColor");
	glUniform3fv(loc, 1, glm::value_ptr(in_light.color));

	loc = glGetUniformLocation(in_renderProgram, "ambientColor");
	glUniform3fv(loc, 1, glm::value_ptr(ambientLightColor));

	glBindVertexArray(in_model.VAO);
	glDrawElements(GL_TRIANGLES, in_model.indexCount, GL_UNSIGNED_INT, nullptr);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void QuickFunc::drawBuffer(programID in_renderProgram, Camera in_camera, GLdata in_model, FrameBuffer in_sorceBuffer) {
	glUseProgram(in_renderProgram);

	unsigned int projectViewUniform = glGetUniformLocation(in_renderProgram, "ProjectionView");
	glUniformMatrix4fv(projectViewUniform, 1, GL_FALSE, glm::value_ptr(in_camera.getProjectionView()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_sorceBuffer.textureID);


	unsigned int loc = glGetUniformLocation(in_renderProgram, "diffuseMap");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(in_renderProgram, "cameraPos");
	mat4 cameraTransform = in_camera.getWorldTransform();
	glUniform3fv(loc, 1, glm::value_ptr(cameraTransform[3]));

	//loc = glGetUniformLocation(in_renderProgram, "lightDirection");
	//glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(0, 0, 0)));

	//loc = glGetUniformLocation(in_renderProgram, "lightColor");
	//glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(0, 0, 0)));

	//loc = glGetUniformLocation(in_renderProgram, "ambientColor");
	//glUniform3fv(loc, 1, glm::value_ptr(ambientLightColor));

	glBindVertexArray(in_model.VAO);
	glDrawElements(GL_TRIANGLES, in_model.indexCount, GL_UNSIGNED_INT, nullptr);
}

void QuickFunc::clearFrameBuffer(FrameBuffer in_framebuffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, in_framebuffer.FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLdata QuickFunc::makeAThing() {
	return GLdata();
}
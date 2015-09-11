#include "quickGLfuncs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"



GLdata QuickFunc::GenerateGrid(unsigned int rows, unsigned int cols) {

	//generate positions
	GLdata dataOut;
	Vertex* aoVertices = new Vertex[rows * cols];
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			aoVertices[r * cols + c].position = vec4(
													(float)c, 0, (float)r, 1);

			glm::vec3 color = glm::vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
			aoVertices[r * cols + c].color = vec4(color, 1);
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
	
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataOut.indexCount * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (unsigned int i = 0; i < in_target->glInfo.size(); i++) {
		glBindVertexArray(in_target->glInfo[i].VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

void QuickFunc::renderTex(programID renderProgram, mat4 projViewMat, GLdata in_target, Texture* in_texture) {
	glUseProgram(renderProgram);

	unsigned int projectViewUniform = glGetUniformLocation(renderProgram, "ProjectionView");
	glUniformMatrix4fv(projectViewUniform, 1, GL_FALSE, glm::value_ptr(projViewMat));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, in_texture->textureID);

	unsigned int loc = glGetUniformLocation(renderProgram, "diffuse");
	glUniform1i(loc, 0);

	glBindVertexArray(in_target.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
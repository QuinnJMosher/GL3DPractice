#include "quickGLfuncs.h"

GLdata QuickFunc::GenerateGrid(unsigned int rows, unsigned int cols) {

	//generate positions
	Vertex* aoVertices = new Vertex[rows * cols];

	for (unsigned int r = 0; r < rows; r++) {
		for (unsigned int c = 0; c < cols; c++) {
			aoVertices[r * cols + c].position = vec4(
													(float)c, 0, (float)r, 1);

			glm::vec3 color = glm::vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
			aoVertices[r * cols + c].color = vec4(color, 1);
		}
	}

	//generate indices
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	
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

	GLdata dataOut;

	dataOut.indexCount = (rows - 1) * (cols - 1) * 6;

	//generate buffers
	glGenBuffers(1, &(dataOut.VBO));
	glGenBuffers(1, &(dataOut.IBO));

	glGenVertexArrays(1, &(dataOut.VAO));

	glBindVertexArray(dataOut.VAO);

	//create vbo
	glBindBuffer(GL_ARRAY_BUFFER, dataOut.VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex),
				aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	///unbind vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//create ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dataOut.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataOut.indexCount * sizeof(unsigned int),
				auiIndices, GL_STATIC_DRAW);

	///unbind ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	///unbind vbo
	glBindVertexArray(0);

	delete[] aoVertices;

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

void QuickFunc::EasyReder(programID renderProgram, mat4 projViewMat, GLdata rednerData) {
	glUseProgram(renderProgram);

	unsigned int projectionViewuniform = glGetUniformLocation(renderProgram, "ProjectionView");
	glUniformMatrix4fv(projectionViewuniform, 1, false, glm::value_ptr(projViewMat));

	glBindVertexArray(rednerData.VAO);
	glDrawElements(GL_TRIANGLES, rednerData.indexCount, GL_UNSIGNED_INT, 0);
}
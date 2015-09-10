#include <OBJHandler.h>

Camera* Geometry::mainCamera;
unsigned int Geometry::shaderProgram;

Geometry::Geometry() { }

void Geometry::ReadyShaderProgram() {
	// create shaders
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Colour; \
							out vec4 vColour; \
							uniform mat4 ProjectionView; \
							void main() { vColour = Colour; gl_Position = ProjectionView * Position;}";
	const char* fsSource = "#version 410\n \
							in vec4 vColour; \
							out vec4 FragColor; \
							void main() { FragColor = vColour; }";
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		
		glGetProgramInfoLog(shaderProgram, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}

void Geometry::SetMainCamera(Camera* in_mainCamera) {
	mainCamera = in_mainCamera;
}

Geometry* Geometry::Load(const string in_filename) {
	Geometry* newInstance = new Geometry();
	newInstance->filename = in_filename;

	string err = "";
	err = tinyobj::LoadObj(newInstance->shapes, newInstance->materials, in_filename.c_str());
	if (err.compare("")) {
		return nullptr;
	}

	newInstance->createGLBuffers();

	return newInstance;

}

void Geometry::Render() {
	glUseProgram(shaderProgram);

	int viewProjectUniform = glGetUniformLocation(shaderProgram, "projection_view");

	glUniformMatrix4fv(viewProjectUniform, 1, GL_FALSE, (float*)&mainCamera->getProjectionView());
	for (unsigned int i = 0; i < shapeInfo.size(); i++) {
		glBindVertexArray(shapeInfo[i].VAO);
		glDrawElements(GL_TRIANGLES, shapeInfo[i].indexCount, GL_UNSIGNED_INT, 0);
	}
}

void Geometry::createGLBuffers() {
	shapeInfo.resize(shapes.size());

	for (unsigned int mesh_index = 0;
	mesh_index < shapes.size();
		++mesh_index)
	{
		glGenVertexArrays(1, &shapeInfo[mesh_index].VAO);
		glGenBuffers(1, &shapeInfo[mesh_index].VBO);
		glGenBuffers(1, &shapeInfo[mesh_index].IBO);
		glBindVertexArray(shapeInfo[mesh_index].VAO);

		unsigned int float_count = shapes[mesh_index].mesh.positions.size();
		float_count += shapes[mesh_index].mesh.normals.size();
		float_count += shapes[mesh_index].mesh.texcoords.size();

		std::vector<float> vertex_data;
		vertex_data.reserve(float_count);

		vertex_data.insert(vertex_data.end(),
			shapes[mesh_index].mesh.positions.begin(),
			shapes[mesh_index].mesh.positions.end());

		vertex_data.insert(vertex_data.end(),
			shapes[mesh_index].mesh.normals.begin(),
			shapes[mesh_index].mesh.normals.end());

		shapeInfo[mesh_index].indexCount =
			shapes[mesh_index].mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, shapeInfo[mesh_index].VBO);
		glBufferData(GL_ARRAY_BUFFER,
			vertex_data.size() * sizeof(float),
			vertex_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shapeInfo[mesh_index].IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int),
			shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normal data

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0,
			(void*)(sizeof(float)*shapes[mesh_index].mesh.positions.size()));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
//base includes
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
//extention Inculdes
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

//declare glm types
using glm::vec3;
using glm::vec4;
using glm::mat4;

//prototypes
int Startup();
int ShutDown();
void Run();

//vars
//settings
//window
const int set_window_width = 1280;
const int set_window_height = 720;
const char* set_window_name = "I am a Window";
//clear color
const float set_clearScr_r = 0.25f;
const float set_clearScr_g = 0.25f;
const float set_clearScr_b = 0.25f;
const float set_clearScr_a = 1;

//running vars
GLFWwindow* window;
mat4 view;
mat4 projection;

int main() {
	int startupErr = Startup();
	if (startupErr != 0) {
		return startupErr;
	}

	Run();

	return ShutDown();
}

int Startup() {
	//startup glfw
	if (glfwInit() == false) {
		return -1;
	}

	//create window
	window = glfwCreateWindow(set_window_width, set_window_height, set_window_name, nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	//load opengl funcs
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		ShutDown();
		return -3;
	}

	//ready Gizmos
	Gizmos::create();

	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	//settings
	glClearColor(set_clearScr_r, set_clearScr_g, set_clearScr_b, set_clearScr_a);

	//if all good
	return 0;
}

int ShutDown() {

	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	//if all good
	return 0;
}


void Run() {
	while (glfwWindowShouldClose(window) == false) {
		//clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glfw update
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
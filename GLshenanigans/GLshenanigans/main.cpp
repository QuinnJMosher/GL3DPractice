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
const float set_clearScr_r = 0.35f;
const float set_clearScr_g = 0.35f;
const float set_clearScr_b = 0.35f;
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
	/*glfwWindow**/ window = glfwCreateWindow(set_window_width, set_window_height, set_window_name, nullptr, nullptr);
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

	/*mat4*/ view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	/*mat4*/ projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

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
		Gizmos::clear();

		//calculate
		Gizmos::addTransform(glm::mat4(1));

		vec4 white(1);
		vec4 black(0, 0, 0, 1);

		for (int i = 0; i < 21; i++) {
			Gizmos::addLine(vec3(-10 + i, 0, 10),
							vec3(-10 + i, 0, -10),
							i == 10 ? white : black);
			Gizmos::addLine(vec3(10, 0, -10 + i),
							vec3(-10, 0, -10 + i),
							i == 10 ? white : black);
		}

		//draw
		Gizmos::draw(projection * view);

		//glfw update
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
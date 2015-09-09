#ifndef _Application_h_
#define _Application_h_

//base includes
#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
//extention Inculdes
#include "Gizmos.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
//my classes
#include "Planet.h"
#include "Camera.h"
#include "FlyCamera.h"
#include "quickGLfuncs.h"

//declare glm types
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Application {
public:
	Application();
	~Application();

	bool Start();
	void Shutdown();
	bool Update();
	void Draw();

private:
	//helper meathods
	//easy drawing
	void DrawGrid();
	void DrawCentre();

	//logic vars
	FlyCamera camera;
	Planet planet;
	Planet moon;

	programID renderProg;
	GLdata grid;

	//active variables
	GLFWwindow* window;
	mat4 view;
	mat4 projection;

	//time vars
	float totalTime;
	float lastTime;
	float deltaTime;

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
	//grid/visuals
	const bool drawsGrid = false;
	const bool drawsCentre = false;
};

#endif

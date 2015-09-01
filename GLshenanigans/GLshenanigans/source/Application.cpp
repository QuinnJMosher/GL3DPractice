#include <Application.h>

Application::Application() { }
Application::~Application() { }

bool Application::Start() {
	//startup glfw
	if (glfwInit() == false) {
		return false;
	}

	//create window
	/*glfwWindow**/ window = glfwCreateWindow(set_window_width, set_window_height, set_window_name, nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	//load opengl funcs
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwTerminate();
		glfwDestroyWindow(window);
		return false;
	}

	//ready Gizmos
	Gizmos::create();

	/*mat4*/ view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	/*mat4*/ projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	//settings
	glClearColor(set_clearScr_r, set_clearScr_g, set_clearScr_b, set_clearScr_a);

	//ready planets
	planet = Planet(vec3(0, 0, 0), 1);

	//if all good
	return true;
}

void Application::Shutdown() {
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Application::Update()
{
	if (glfwWindowShouldClose(window)) {
		return false;
	}

	planet.update(0);

	return true;
}

void Application::Draw() {

	//clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//reset objects
	Gizmos::clear();

	//add objects here
	if (drawsGrid) {
		DrawGrid();
	}

	if (drawsCentre) {
		DrawCentre();
	}

	Planet::Draw(planet);

	//draw
	Gizmos::draw(projection * view);

	//glfw update
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Application::DrawGrid() {
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
}

void Application::DrawCentre() {
	Gizmos::addTransform(glm::mat4(1));
}
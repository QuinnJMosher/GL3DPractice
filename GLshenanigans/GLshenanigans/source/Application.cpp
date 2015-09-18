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

	Input::Init(window);
	
	//fly cam only
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//camera.inputContext = window;
	camera.setSpeed(10.0f);
	camera.rotateSensitivity = 0.5f;
	//all camera
	camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	camera.setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	//settings
	glClearColor(set_clearScr_r, set_clearScr_g, set_clearScr_b, set_clearScr_a);

	glEnable(GL_DEPTH_TEST);

	lastTime = glfwGetTime();
	//renderProg = QuickFunc::QuickTextProg();
	//renderProg = QuickFunc::makeProgram("./assets/shaders/textureVertex.glsl", "./assets/shaders/textureFragment.glsl");
	renderProg = QuickFunc::makeProgram("./assets/shaders/lightVertex.glsl", "./assets/shaders/lightFragment.glsl");
	//renderProg = QuickFunc::makeProgram("./assets/shaders/tutVertex.glsl", "./assets/shaders/tutFragment.glsl");
	//grid = QuickFunc::GenerateGrid(10, 10);
	grid = QuickFunc::LoadFBX("./assets/soulspear/soulspear.fbx");

	//geo = QuickFunc::loadGeometry("./assets/dragon.obj");
	//tex = QuickFunc::LoadTexture("./assets/crate.png");
	tex = QuickFunc::LoadFBXTexture("./assets/soulspear/soulspear.fbx");

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

	totalTime = glfwGetTime();
	deltaTime = totalTime - lastTime;

	camera.update(deltaTime);

	Input::Update();

	lastTime = totalTime;
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

	//QuickFunc::EasyReder(renderProg, camera.getProjectionView(), grid, totalTime);
	//QuickFunc::renderGeo(renderProg, camera.getProjectionView(), geo);
	QuickFunc::renderTex(renderProg, camera, grid, tex);

	//visualize directional light
	Gizmos::addTransform(glm::translate(vec3(-1, -1, -0.5)));

	//draw
	Gizmos::draw(camera.getProjectionView());
	
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
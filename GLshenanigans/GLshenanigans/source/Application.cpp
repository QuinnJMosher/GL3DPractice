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
	camera.setSpeed(2.0f);
	camera.rotateSensitivity = 0.2f;
	//all camera
	camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	camera.setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	stillCamera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	stillCamera.setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	//settings
	glClearColor(set_clearScr_r, set_clearScr_g, set_clearScr_b, set_clearScr_a);

	glEnable(GL_DEPTH_TEST);

	lastTime = glfwGetTime();
	
	simpleProg = QuickFunc::makeProgram("./assets/shaders/textureVertex.glsl", "./assets/shaders/textureFragment.glsl");
	renderProg = QuickFunc::makeProgram("./assets/shaders/normalMapingV.glsl", "./assets/shaders/normalMapingF.glsl");
	postProg = QuickFunc::makeProgram("./assets/shaders/postVert.glsl", "./assets/shaders/postFrag.glsl");
	
	//buffDisplay = QuickFunc::LoadFBX("./assets/cube.fbx");
	grid = QuickFunc::LoadFBX("./assets/soulspear/soulspear.fbx");

	tex = QuickFunc::LoadFBXTexture("./assets/soulspear/soulspear.fbx", 0);
	normalMap = QuickFunc::LoadFBXTexture("./assets/soulspear/soulspear.fbx", 1);

	buffDisplay = QuickFunc::ReadyPostProcessing(set_window_width, set_window_height);

	light.direction = vec3(-1.0f, -1.0f, -0.5f);
	light.color = vec3(0.5f, 0.5f, 0.5f);

	frameBuff = QuickFunc::createFrameBuffer(set_window_width, set_window_height);
	/*QuickFunc::clearFrameBuffer(frameBuff);*/
	/*QuickFunc::drawToBuffer(simpleProg, camera, grid, tex, normalMap, light, frameBuff);*/

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

	if (Input::IsKey(KeyCode::KP_1, KeyState::Pressed)) {
		light.direction = vec3(-1.0f, -1.0f, -0.5f);
	}
	if (Input::IsKey(KeyCode::KP_2, KeyState::Pressed)) {
		light.direction = vec3(0.5f, 0.5f, 0.5f);
	}
	if (Input::IsKey(KeyCode::KP_3, KeyState::Pressed)) {
		light.direction = vec3(-0.5f, -0.5f, -0.5f);
	}
	if (Input::IsKey(KeyCode::KP_4, KeyState::Pressed)) {
		light.direction = vec3(-1.0f, 1.0f, -0.5f);
	}
	if (Input::IsKey(KeyCode::KP_5, KeyState::Pressed)) {
		light.direction = vec3(1.0f, -1.0f, -0.5f);
	}
	if (Input::IsKey(KeyCode::KP_6, KeyState::Pressed)) {
		light.direction = vec3(-1.0f, -1.0f, 0.5f);
	}

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

	//put things on the fbo
	QuickFunc::clearFrameBuffer(frameBuff);
	QuickFunc::drawToBuffer(renderProg, camera, grid, tex, normalMap, light, frameBuff);

	//put fbo on screen
	QuickFunc::DrawPostProcessing(frameBuff, buffDisplay, postProg);

	////visualize directional light
	//vec3 lightSource = glm::normalize(-(light.direction)) * 10;
	//Gizmos::addTransform(glm::translate(lightSource));

	//draw
	//Gizmos::draw(camera.getProjectionView());
	
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
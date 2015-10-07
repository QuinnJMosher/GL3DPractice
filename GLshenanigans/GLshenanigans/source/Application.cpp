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

	//deff rendering shenanigans
	//create geoBuff
	geoBuff.imageWidth = set_window_width;
	geoBuff.imageHeight = set_window_height;

	geoBuff_posTex.imageWidth = set_window_width;
	geoBuff_posTex.imageHeight = set_window_height;

	geoBuff_normTex.imageWidth = set_window_width;
	geoBuff_normTex.imageHeight = set_window_height;

	//generate
	glGenFramebuffers(1, &geoBuff.FBO);
	glGenTextures(1, &geoBuff.textureID);
	glGenTextures(1, &geoBuff_posTex.textureID);
	glGenTextures(1, &geoBuff_normTex.textureID);
	glGenRenderbuffers(1, &geoBuff.RenderBuffer);

	//bind
	glBindFramebuffer(GL_FRAMEBUFFER, geoBuff.FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, geoBuff.RenderBuffer);

	//setuptextures
	glBindTexture(GL_TEXTURE_2D, geoBuff.textureID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, set_window_width, set_window_height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, geoBuff_posTex.textureID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, set_window_width, set_window_height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, geoBuff_normTex.textureID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, set_window_width, set_window_height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//setup renderbuffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, set_window_width, set_window_height);

	//associate buffers
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, geoBuff.textureID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, geoBuff_posTex.textureID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, geoBuff_normTex.textureID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, geoBuff.RenderBuffer);

	GLenum geoBuffTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, geoBuffTargets);

	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//create lightBuff
	lightBuff.imageWidth = set_window_width;
	lightBuff.imageHeight = set_window_height;

	//generate
	glGenFramebuffers(1, &lightBuff.FBO);
	glGenTextures(1, &lightBuff.textureID);
	glGenRenderbuffers(1, &lightBuff.RenderBuffer);

	//bind
	glBindFramebuffer(GL_FRAMEBUFFER, lightBuff.FBO);
	glBindTexture(GL_TEXTURE_2D, lightBuff.textureID);
	glBindRenderbuffer(GL_RENDERBUFFER, lightBuff.RenderBuffer);

	//setup
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, set_window_width, set_window_height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, lightBuff.textureID, 0);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, set_window_width, set_window_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, lightBuff.RenderBuffer);

	GLenum lightBuffTargets[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightBuffTargets);

	//checkstatus
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("framebuffer error!\n");
	}

	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	renderPlane = QuickFunc::ReadyPostProcessing(set_window_width, set_window_height);

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
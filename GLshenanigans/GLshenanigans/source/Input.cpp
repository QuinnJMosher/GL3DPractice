#include <Input.h>
#include <GLFW\glfw3.h>

vec2 Input::lastMousePos = vec2(0, 0);
vec2 Input::curMousePos = vec2(0, 0);
vec2 Input::deltaMousePos = vec2(0, 0);
KeyState Input::curMouseStates[8];
KeyState Input::curKeyStates[348];
GLFWwindow* Input::context = nullptr;

void Input::Init(GLFWwindow* in_context) {
	context = in_context;
}

void Input::Update() {

	//take and handle keystates for keys
	for (int i = 0; i < KeyCode::KB_LAST; i++) {
		//get keyState
		bool isCurrentDown = (glfwGetKey(context, i) == GLFW_PRESS);
		if (isCurrentDown) {
			//if the key IS down but wasent on the last frame 
			if (curKeyStates[i] == KeyState::Up || curKeyStates[i] == KeyState::Released) {
				curKeyStates[i] = KeyState::Pressed;//then it has just been pressed
			}//if the key was pressed last frame and still is down then it is now "Down"
			else if (curKeyStates[i] == KeyState::Pressed) {
				curKeyStates[i] = KeyState::Down;
			}
		}
		else
		{
			//if the key isnt down ad it was last frame
			if (curKeyStates[i] == KeyState::Down || curKeyStates[i] == KeyState::Pressed) {
				curKeyStates[i] = KeyState::Released;//then it has just been released
			}//if the key was realeased last frame an is still up then ut us now "Up"
			else if (curKeyStates[i] == KeyState::Released) {
				curKeyStates[i] = KeyState::Up;
			}
		}
	}

	//take and handle keystates for buttons
	for (int i = 0; i < MouseButtonCode::MO_LAST; i++) {
		bool isCurrentDown = (glfwGetKey(context, i) == GLFW_PRESS);
		if (isCurrentDown) {
			if (curMouseStates[i] == KeyState::Up || curMouseStates[i] == KeyState::Released) {
				curMouseStates[i] = KeyState::Pressed;
			}
			else if (curMouseStates[i] == KeyState::Pressed) {
				curMouseStates[i] = KeyState::Down;
			}
		}
		else
		{
			if (curMouseStates[i] == KeyState::Down || curMouseStates[i] == KeyState::Pressed) {
				curMouseStates[i] = KeyState::Released;
			}
			else if (curMouseStates[i] == KeyState::Released) {
				curMouseStates[i] = KeyState::Up;
			}
		}
	}

	//handle mouse position
	lastMousePos = vec2(curMousePos);
	double curMouseX, curMouseY;
	glfwGetCursorPos(context, &curMouseX, &curMouseY);
	curMousePos.x = curMouseX;
	curMousePos.y = curMouseY;
	deltaMousePos = vec2(curMouseX - lastMousePos.x, curMouseY - lastMousePos.y);
}

//key stuff

KeyState Input::GetKey(KeyCode in_key) {
	return curKeyStates[in_key];
}

bool Input::IsKey(KeyCode in_key, KeyState in_state) {
	if (in_state == KeyState::Up) {
		return (curKeyStates[in_key] == KeyState::Up || curKeyStates[in_key] == KeyState::Released);
	}
	else if (in_state == KeyState::Down) {
		return (curKeyStates[in_key] == KeyState::Down || curKeyStates[in_key] == KeyState::Pressed);
	}
	return (curKeyStates[in_key] == in_state);
}

//mouse stuff

KeyState Input::GetMouseButton(MouseButtonCode in_button) {
	return curMouseStates[in_button];
}

bool Input::IsMouseButton(MouseButtonCode in_button, KeyState in_state) {
	if (in_state == KeyState::Up) {
		return (curMouseStates[in_button] == KeyState::Up || curMouseStates[in_button] == KeyState::Released);
	}
	else if (in_state == KeyState::Down) {
		return (curMouseStates[in_button] == KeyState::Down || curMouseStates[in_button] == KeyState::Pressed);
	}
	return (curMouseStates[in_button] == in_state);
}

vec2 Input::GetMousePos() {
	return curMousePos;
}

vec2 Input::GetMouseDelta() {
	return deltaMousePos;
}
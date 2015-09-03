#include <FlyCamera.h>

void FlyCamera::update(float in_deltaTime) {
	//get directional input
	float input_RL = 0;//right is positive
	float input_UD = 0;//up is positive
	if (glfwGetKey(inputContext, GLFW_KEY_W) == GLFW_PRESS) {
		input_UD += (speed * in_deltaTime);
	}
	if (glfwGetKey(inputContext, GLFW_KEY_S) == GLFW_PRESS) {
		input_UD -= (speed * in_deltaTime);
	}
	if (glfwGetKey(inputContext, GLFW_KEY_A) == GLFW_PRESS) {
		input_RL -= (speed * in_deltaTime);
	}
	if (glfwGetKey(inputContext, GLFW_KEY_D) == GLFW_PRESS) {
		input_RL += (speed * in_deltaTime);
	}

	//get rotational input
	float input_rotate_UD = 0;//up is positive
	float input_rotate_RL = 0;//right is positive
	if (glfwGetKey(inputContext, GLFW_KEY_UP) == GLFW_PRESS) {
		input_rotate_UD += 1;
	}
	if (glfwGetKey(inputContext, GLFW_KEY_DOWN) == GLFW_PRESS) {
		input_rotate_UD -= 1;
	}
	if (glfwGetKey(inputContext, GLFW_KEY_LEFT) == GLFW_PRESS) {
		input_rotate_RL -= 1;
	}
	if (glfwGetKey(inputContext, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		input_rotate_RL += 1;
	}

	//create the movement mat
	mat4 totalMovementMat = glm::translate((forward * input_UD) + (right * input_RL));

	worldTransform = totalMovementMat * worldTransform;

	//get current pos
	vec3 currentPos = vec3(worldTransform[3]);
	//matke turn amt a vector;
	vec3 inFrontOfMe = ((forward * 10) + currentPos);
	vec3 turnRightLeft = glm::rotate(forward, (rotateSensitivity * in_deltaTime) * input_rotate_UD, right);
	vec3 turnUpDown = glm::rotate(forward, (rotateSensitivity * in_deltaTime) * -input_rotate_RL, up);
	//turn camera
	this->setLookAt(currentPos, inFrontOfMe + (turnRightLeft + turnUpDown) * 10, up);
	
}
void FlyCamera::setSpeed(float in_speed) {
	speed = in_speed;
}
void FlyCamera::setLookAt(vec3 in_from, vec3 in_to, vec3 in_up) {
	up = in_up;
	forward = glm::normalize((in_from - in_to) * -1);
	right = glm::cross(forward, up);
	this->Camera::setLookAt(in_from, in_to, in_up);

}
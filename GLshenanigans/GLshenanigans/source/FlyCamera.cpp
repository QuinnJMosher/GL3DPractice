#include <FlyCamera.h>

void FlyCamera::update(float in_deltaTime) {
	float input_RL = 0;//right is positive
	float input_UD = 0;//up is positive
	//get input
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

	//create the movement vector
	mat4 totalMovementMat = glm::translate((forward * input_UD) + (right * input_RL));

	worldTransform = totalMovementMat * worldTransform;
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
#include <FlyCamera.h>

void FlyCamera::update(float in_deltaTime) {
	//get directional input
	float input_RL = 0;//right is positive
	float input_UD = 0;//up is positive
	if (Input::IsKey(KeyCode::W, KeyState::Down)) {
		input_UD += (speed * in_deltaTime);
	}
	if (Input::IsKey(KeyCode::S, KeyState::Down)) {
		input_UD -= (speed * in_deltaTime);
	}
	if (Input::IsKey(KeyCode::A, KeyState::Down)) {
		input_RL -= (speed * in_deltaTime);
	}
	if (Input::IsKey(KeyCode::D, KeyState::Down)) {
		input_RL += (speed * in_deltaTime);
	}
	
	//get rotational input
	//get mouse pos
	vec2 deltaMouse = Input::GetMouseDelta();
	//create the movement mat
	mat4 totalMovementMat = glm::translate((forward * input_UD) + (right * input_RL));

	worldTransform = totalMovementMat * worldTransform;

	//get current pos
	vec3 currentPos = vec3(worldTransform[3]);
	//matke turn amt a vector;
	vec3 inFrontOfMe = ((forward * 10) + currentPos);
	vec3 turnRightLeft = glm::rotate(forward, (rotateSensitivity * in_deltaTime) * -deltaMouse.y, right);
	vec3 turnUpDown = glm::rotate(forward, (rotateSensitivity * in_deltaTime) * -deltaMouse.x, up);
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
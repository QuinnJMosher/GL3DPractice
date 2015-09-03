#include <Camera.h>

void Camera::update(float in_deltaTime) {

}
void Camera::setPerspective(float in_FOV, float in_aspectRatio, float in_near, float in_far) {
	projectionTransform = glm::perspective(in_FOV, in_aspectRatio, in_near, in_far);
}
void Camera::setLookAt(vec3 in_from, vec3 in_to, vec3 in_up) {
	worldTransform = glm::inverse(glm::lookAt(in_from, in_to, in_up));
}
void Camera::setPosition(vec3 in_position) {
	worldTransform = glm::translate(in_position);
}
mat4 Camera::getWorldTransform() {
	return worldTransform;
}
mat4 Camera::getView() {
	return glm::inverse(worldTransform);
}
mat4 Camera::getProjection() {
	return projectionTransform;
}
mat4 Camera::getProjectionView() {
	return projectionTransform * glm::inverse(worldTransform);
}
//void Camera::updateProjectionViewTransform();
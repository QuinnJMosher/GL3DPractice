#include <Planet.h>

int Planet::detail = 10;

Planet::Planet() {
	position = glm::vec3();
	radius = 1;
	color = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	rotation = glm::vec3();
	transfomMat = glm::mat4();
}

Planet::Planet(const glm::vec3 in_position, const float in_radius, const glm::vec4 in_color) {
	position = in_position;
	radius = in_radius;
	color = in_color;

	transfomMat = glm::mat4();
}

Planet::~Planet() { }

void Planet::update(float in_time) {
	//orbit then rotation

	//create rotation mats
	glm::mat4 rotationMat = glm::rotate(1.0f, rotation);

	transfomMat = rotationMat;//evrybody mult'ed together

}

void Planet::Draw(Planet& ref_target) {
	Gizmos::addSphere(ref_target.position, ref_target.radius, detail, detail, ref_target.color, &ref_target.transfomMat);
}
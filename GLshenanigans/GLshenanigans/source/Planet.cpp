#include <Planet.h>

int Planet::detail = 10;

Planet::Planet() {
	position = glm::vec3();
	radius = 1;
	color = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	rotation = glm::vec3();

	globalMat = glm::mat4();
	localMat = glm::mat4();
	parentMat = nullptr;
}

Planet::Planet(const glm::vec3 in_position, const float in_radius, const glm::vec4 in_color) {
	position = in_position;
	radius = in_radius;
	color = in_color;

	globalMat = glm::mat4();
	localMat = glm::mat4();
	parentMat = nullptr;
}

Planet::~Planet() { }

void Planet::update(float in_time) {
	//orbit then rotation

	//global
	//transform
	//orbit
	//local
	//rotate

}

void Planet::Draw(Planet& ref_target) {
	Gizmos::addSphere(ref_target.position, ref_target.radius, detail, detail, ref_target.color, &ref_target.transfomMat);
}
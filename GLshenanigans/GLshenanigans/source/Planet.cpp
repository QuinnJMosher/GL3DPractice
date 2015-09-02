#include <Planet.h>

int Planet::detail = 10;

Planet::Planet() {
	position = glm::vec3();
	radius = 1;
	color = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	rotation = glm::vec3(0, 1, 0);

	globalMat = glm::mat4();
	localMat = glm::mat4();
	parent = nullptr;
}

Planet::Planet(const glm::vec3 in_position, const float in_radius, const glm::vec4 in_color) {
	position = in_position;
	radius = in_radius;
	color = in_color;

	rotation = glm::vec3(0, 1, 0);

	globalMat = glm::mat4();
	localMat = glm::mat4();
	parent = nullptr;
}

Planet::~Planet() { }

void Planet::update(float in_time) {
	//orbit then rotation

	//global
	//translate
	glm::mat4 translateMat = glm::translate(position);
	//orbit
	if (parent != nullptr) {
		globalMat = translateMat * glm::rotate(glm::mat4(), in_time, vec3(0, 1, 0)) * parent->globalMat;
	}
	else {
		globalMat = translateMat;
	}

	//local
	//rotate
	localMat = glm::rotate(glm::mat4(), in_time, rotation);

	//globalMat = glm::translate(position);
	//localMat = glm::rotate(glm::mat4(), in_time, vec3(0, 1, 0));
}

void Planet::Draw(Planet& ref_target) {
	glm::mat4 totalmat = ref_target.globalMat * ref_target.localMat;
	Gizmos::addSphere(vec3(0), ref_target.radius, detail, detail, ref_target.color, &totalmat);
}
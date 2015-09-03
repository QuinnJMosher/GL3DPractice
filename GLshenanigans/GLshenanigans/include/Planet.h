#ifndef _Planet_h_
#define _Planet_h_

//shape include
#include <Gizmos.h>
//math includes
#include <glm/glm.hpp>
#include <glm/ext.hpp>
//math defines
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Planet {
public:
	Planet();
	Planet(const glm::vec3 in_position, const float in_radius, const glm::vec4 in_color = glm::vec4(0));
	~Planet();

	void update(float in_time);
	static void Draw(Planet& ref_target);

	glm::vec3 position;
	float radius;
	glm::vec4 color;
	glm::vec3 rotation;
	Planet* parent;
	float orbit;
	glm::mat4 globalMat;
	glm::mat4 localMat;
	static int detail;
};

#endif
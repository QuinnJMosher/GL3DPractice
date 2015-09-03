#ifndef _Camera_h_
#define _Camera_h_
//glm
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::mat4;

class Camera {
public:
	virtual void update(float in_deltaTime);
	void setPerspective(float in_FOV, float in_aspectRatio, float in_near, float in_far);
	void setLookAt(vec3 in_from, vec3 in_to, vec3 in_up);
	void setPosition(vec3 in_position);
	mat4 getWorldTransform();
	mat4 getView();
	mat4 getProjection();
	mat4 getProjectionView();
	//void updateProjectionViewTransform();
private:
	mat4 worldTransform;
	//mat4 viewTransform;
	mat4 projectionTransform;
	//mat4 projectionView;
};

#endif

#ifndef _FlyCamera_h_
#define _FlyCamera_h_

#include <Camera.h>//parent

//glfw for input 'cause i'm lazy
#include <GLFW\glfw3.h>

class FlyCamera : public Camera
{
public:
	virtual void update(float in_deltaTime);
	void setSpeed(float in_speed);
	virtual void setLookAt(vec3 in_from, vec3 in_to, vec3 in_up);

	GLFWwindow* inputContext;
	float rotateSensitivity;
private:
	float speed;
	//float rotateSensitivity;
	vec3 up;
	vec3 forward;
	vec3 right;
	
};

#endif
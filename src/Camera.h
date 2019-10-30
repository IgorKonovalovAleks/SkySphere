#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma once
class Camera
{
private:
	const glm::vec3 vecUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 position;
	glm::vec3 front;
	GLfloat speed;

public:
	Camera();
	glm::mat4 getViewMatrix();
	void move(bool forward, bool backward, bool up, bool down, bool left, bool right);
};


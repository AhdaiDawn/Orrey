#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class
{
public:
	glm::mat4 getPositionViewMatrix() {
		return glm::lookAt(position, position + Front, Up);
	}

private:
	glm::vec2 mousePos = glm::vec2();
	float zoom = -50.0f;
	float rotationSpeed = 7.5f;
	float movementSpeed = 1.0f;
	bool viewUpdated = false;

	glm::vec3 rotation = glm::vec3();

	glm::vec3 position = glm::vec3();
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	struct
	{
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
		bool accelerate = false;
		bool moderate = false;
	} keys;
} Camera;
#endif

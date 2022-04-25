#include "Camera.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#define PI glm::pi<float>()

Camera::Camera(int w, int h, glm::vec3 initPos)
{
	positionVec3 = glm::vec3(0,0,0);
	this->initPos = initPos;
	projectionMatrix = glm::perspective(PI / 4, (float)w / h, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(initPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Camera::move(Direction dir)
{
	switch (dir)
	{
	case FORWARDS:
		positionVec3.z -= moveSpeed;
		break;
	case BACKWARDS:
		positionVec3.z += moveSpeed;
		break;
	case UP:
		positionVec3.y -= moveSpeed;
		break;
	case DOWN:
		positionVec3.y += moveSpeed;
		break;
	case LEFT:
		positionVec3.x -= moveSpeed;
		break;
	case RIGHT:
		positionVec3.x += moveSpeed;
		break;
	}
}

void Camera::rotate(Direction dir)
{
	switch (dir)
	{
	case FORWARDS:
		rotationVec3.z -= rotationSpeed;
		break;
	case BACKWARDS:
		rotationVec3.z += rotationSpeed;
		break;
	case UP:
		rotationVec3.y -= rotationSpeed;
		break;
	case DOWN:
		rotationVec3.y += rotationSpeed;
		break;
	case LEFT:
		rotationVec3.x -= rotationSpeed;
		break;
	case RIGHT:
		rotationVec3.x += rotationSpeed;
		break;
	}
}

glm::mat4 Camera::getViewMatrix() 
{
	viewMatrix = glm::lookAt(initPos + positionVec3, positionVec3 + rotationVec3, glm::vec3(0, 1, 0));
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}


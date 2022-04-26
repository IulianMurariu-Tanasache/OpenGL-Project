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
	cameraPos = initPos;
	cameraUp = glm::vec3(0, 1, 0);
	cameraFront = glm::vec3(0, 0, -1);
	rotationVec3 = glm::vec3(-90.0f, 0, 0);
	projectionMatrix = glm::perspective(PI / 4, (float)w / h, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(cameraPos,cameraPos + cameraFront, cameraUp);
}

void Camera::move(Direction dir, int deltaTime)
{
	switch (dir)
	{
	case FORWARDS:
		cameraPos += moveSpeed * cameraFront * (deltaTime / 1000.0f);
		break;
	case BACKWARDS:
		cameraPos -= moveSpeed * cameraFront * (deltaTime / 1000.0f);
		break;
	case UP:
		cameraPos -= moveSpeed * cameraUp * (deltaTime / 1000.0f);
		break;
	case DOWN:
		cameraPos += moveSpeed * cameraUp * (deltaTime / 1000.0f);
		break;
	case LEFT:
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed * (deltaTime / 1000.0f);
		break;
	case RIGHT:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed * (deltaTime / 1000.0f);
		break;
	}
}

void Camera::rotate(Direction dir, int deltaTime)
{
	switch (dir)
	{
	case FORWARDS:
		rotationVec3.z -= rotationSpeed * (deltaTime / 1000.0f);
		break;
	case BACKWARDS:
		rotationVec3.z += rotationSpeed * (deltaTime / 1000.0f);
		break;
	case UP:
		rotationVec3.y += rotationSpeed * (deltaTime / 1000.0f);
		break;
	case DOWN:
		rotationVec3.y -= rotationSpeed * (deltaTime / 1000.0f);
		break;
	case LEFT:
		rotationVec3.x -= rotationSpeed * (deltaTime / 1000.0f);
		break;
	case RIGHT:
		rotationVec3.x += rotationSpeed * (deltaTime / 1000.0f);
		break;
	}
	if (rotationVec3.y > 89.0f)
		rotationVec3.y = 89.0f;
	if (rotationVec3.y < -89.0f)
		rotationVec3.y = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(rotationVec3.z)) * cos(glm::radians(rotationVec3.x));
	direction.y = sin(glm::radians(rotationVec3.z)) * sin(glm::radians(rotationVec3.y)); 
	direction.z = sin(glm::radians(rotationVec3.x)) * cos(glm::radians(rotationVec3.y));
	cameraFront = glm::normalize(direction);
}

glm::mat4 Camera::getViewMatrix() 
{
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}


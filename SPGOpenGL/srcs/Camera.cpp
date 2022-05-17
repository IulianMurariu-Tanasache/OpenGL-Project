#include "Camera.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>


void Camera::calcDirection()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(rotationVec3.z)) * cos(glm::radians(rotationVec3.y));
	direction.y = sin(glm::radians(rotationVec3.y));
	direction.z = sin(glm::radians(rotationVec3.z)) * cos(glm::radians(rotationVec3.y));
	cameraFront = glm::normalize(direction);

	direction.x = cos(glm::radians(rotationVec3.z)) * cos(glm::radians(rotationVec3.y + 90));
	direction.y = sin(glm::radians(rotationVec3.y + 90));
	direction.z = sin(glm::radians(rotationVec3.z)) * cos(glm::radians(rotationVec3.y + 90));
	cameraUp = glm::normalize(direction);

	cameraRight = glm::cross(cameraFront, cameraUp);

	frustrum = new Frustrum(cameraPos, cameraFront, cameraRight, cameraUp, zNear, zFar, aspect, fov);
}

Camera::Camera(int w, int h, glm::vec3 initPos, float initPitch, float fov, float zNear, float zFar)
{
	cameraPos = initPos;
	cameraUp = glm::vec3(0, 1, 0);
	cameraFront = glm::vec3(0, 0, -1);
	cameraRight = glm::cross(cameraFront, cameraUp);

	projectionMatrix = glm::perspective(fov, (float)w / h, zNear, zFar);
	viewMatrix = glm::lookAt(cameraPos,cameraPos + cameraFront, cameraUp);

	this->fov = fov;
	this->zNear = zNear;
	this->zFar = zFar;
	this->aspect = (float)w / h;

	this->rotate(DOWN, initPitch);
	this->rotate(LEFT, 90.0f);

	frustrum = new Frustrum(cameraPos, cameraFront, cameraRight, cameraUp, zNear, zFar, aspect, fov);
}

Camera::~Camera()
{
	delete frustrum;
	frustrum = nullptr;
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
		cameraPos += moveSpeed * cameraUp * (deltaTime / 1000.0f);
		break;
	case DOWN:
		cameraPos -= moveSpeed * cameraUp * (deltaTime / 1000.0f);
		break;
	case LEFT:
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed * (deltaTime / 1000.0f);
		break;
	case RIGHT:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed * (deltaTime / 1000.0f);
		break;
	}

	frustrum = new Frustrum(cameraPos, cameraFront, cameraRight, cameraUp, zNear, zFar, aspect, fov);
}



void Camera::rotate(Direction dir, float angle)
{
	switch (dir)
	{
	case FORWARDS:
		rotationVec3.x -= angle;
		break;
	case BACKWARDS:
		rotationVec3.x += angle;
		break;
	case UP:
		rotationVec3.y += angle;
		break;
	case DOWN:
		rotationVec3.y -= angle;
		break;
	case LEFT:
		rotationVec3.z -= angle;
		break;
	case RIGHT:
		rotationVec3.z += angle;
		break;
	}
	calcDirection();
}

void Camera::rotate(Direction dir, int offset)
{
	switch (dir)
	{
	case FORWARDS:
		rotationVec3.x -= offset * rotationSpeed;
		break;
	case BACKWARDS:
		rotationVec3.x += offset * rotationSpeed;
		break;
	case UP:
		rotationVec3.y += offset * rotationSpeed;
		break;
	case DOWN:
		rotationVec3.y -= offset * rotationSpeed;
		break;
	case LEFT:
		rotationVec3.z -= offset * rotationSpeed;
		break;
	case RIGHT:
		rotationVec3.z += offset * rotationSpeed;
		break;
	}
	calcDirection();
	
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


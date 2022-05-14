#pragma once
#include "Plane.h"
#include <glm/gtc/type_ptr.hpp>

class Frustrum {
	public:
		Plane topPlane;
		Plane bottomPlane;
		Plane leftPlane;
		Plane rightPlane;
		Plane nearPlane;
		Plane farPlane;
		
		Frustrum(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraRight, const glm::vec3& cameraUp, float zNear, float zFar, float aspect, float fov);
};
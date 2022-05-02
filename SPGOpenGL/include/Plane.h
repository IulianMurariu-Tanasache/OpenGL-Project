#pragma once
#include <glm/gtc/type_ptr.hpp>

struct Plane {
	
	glm::vec3 normal;
	float distance;

	Plane();
	Plane(glm::vec3 n, float d) : normal(n), distance(d) {};
	Plane(glm::vec3 n, glm::vec3 point) : normal(n) {
		distance = glm::dot(normal, point);
	};
	float getSignedDistanceToPlan(const glm::vec3& point) const;
};

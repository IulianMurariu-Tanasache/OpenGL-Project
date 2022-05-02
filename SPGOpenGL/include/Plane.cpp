#include "Plane.h"

Plane::Plane() {
	normal = { 0.0f, 1.0f, 0.0f };
	distance = 0.0f;
}

float Plane::getSignedDistanceToPlan(const glm::vec3& point) const
{
    return glm::dot(normal, point) - distance;
}

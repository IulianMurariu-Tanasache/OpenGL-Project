#pragma once
#include "BoundingVolume.h"

class BoundingSphere : public Volume
{
	public:
		glm::vec3 center;
		float radius;

		BoundingSphere(glm::vec3 center, float radius) : center(center), radius(radius) {};
		bool isOnFrustrum(const Frustrum& frustrum, glm::mat4 modelMatrix, glm::vec3 globalScale) override;
		bool isOnOrForwardPlan(const Plane& plane) override;
		bool isOnFrustrum(const Frustrum& frustrum, const glm::vec3 pos) override { return false; };//cred
};

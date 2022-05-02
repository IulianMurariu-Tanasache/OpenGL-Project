#pragma once
#include "Frustrum.h"

class Volume
{
	public:
		virtual bool isOnFrustrum(const Frustrum& frustrum, const glm::vec3 pos) = 0;//cred
		virtual bool isOnOrForwardPlan(const Plane& plane) = 0;
		virtual bool isOnFrustrum(const Frustrum& frustrum, glm::mat4 modelMatrix, glm::vec3 globalScale) = 0;
};

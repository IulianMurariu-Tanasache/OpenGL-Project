#include "BoundingSphere.h"
#include <algorithm>

bool BoundingSphere::isOnFrustrum(const Frustrum& frustrum, glm::mat4 modelMatrix, glm::vec3 globalScale)
{
    //Get global scale is computed by doing the magnitude of
    //X, Y and Z model matrix's column.
    //const glm::vec3 globalScale = sqrt(glm::dot({})))//transform.getGlobalScale();

    //Get our global center with process it with the global model matrix of our transform
    const glm::vec3 globalCenter{ modelMatrix * glm::vec4(center, 1.f)};

    //To wrap correctly our shape, we need the maximum scale scalar.
    const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

    //Max scale is assuming for the diameter. So, we need the half to apply it to our radius
    BoundingSphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

    //Check Firstly the result that have the most chance
    //to faillure to avoid to call all functions.
    return (globalSphere.isOnOrForwardPlan(frustrum.leftPlane) &&
        globalSphere.isOnOrForwardPlan(frustrum.rightPlane) &&
        globalSphere.isOnOrForwardPlan(frustrum.farPlane) &&
        globalSphere.isOnOrForwardPlan(frustrum.nearPlane) &&
        globalSphere.isOnOrForwardPlan(frustrum.topPlane) &&
        globalSphere.isOnOrForwardPlan(frustrum.bottomPlane));
}

bool BoundingSphere::isOnOrForwardPlan(const Plane& plane) 
{
	return plane.getSignedDistanceToPlan(center) >= -radius;
}
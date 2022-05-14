#include "Frustrum.h"

Frustrum::Frustrum(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraRight, const glm::vec3& cameraUp, float zNear, float zFar, float aspect, float fov)
{
    const float halfVSide = zFar * tanf(fov * .5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = zFar * cameraFront;

    nearPlane = Plane(cameraFront, cameraPos + zNear * cameraFront);
    farPlane = Plane(-cameraFront, cameraPos + frontMultFar);
    rightPlane = Plane(glm::cross(cameraUp, frontMultFar + cameraRight * halfHSide), cameraPos);
    leftPlane = Plane(glm::cross(frontMultFar - cameraRight * halfHSide, cameraUp), cameraPos);
    topPlane = Plane(glm::cross(cameraRight, frontMultFar - cameraUp * halfVSide), cameraPos);
    bottomPlane = Plane(glm::cross(frontMultFar + cameraUp * halfVSide, cameraRight), cameraPos);
}
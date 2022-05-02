#include "Frustrum.h"

Frustrum::Frustrum(const Camera& cam)
{
    const float halfVSide = cam.zFar * tanf(cam.fov * .5f);
    const float halfHSide = halfVSide * cam.aspect;
    const glm::vec3 frontMultFar = cam.zFar * cam.cameraFront;

    nearPlane = Plane(cam.cameraFront, cam.cameraPos + cam.zNear * cam.cameraFront);
    farPlane = Plane(-cam.cameraFront, cam.cameraPos + frontMultFar);
    rightPlane = Plane(glm::cross(cam.cameraUp, frontMultFar + cam.cameraRight * halfHSide), cam.cameraPos);
    leftPlane = Plane(glm::cross(frontMultFar - cam.cameraRight * halfHSide, cam.cameraUp), cam.cameraPos);
    topPlane = Plane(glm::cross(cam.cameraRight, frontMultFar - cam.cameraUp * halfVSide), cam.cameraPos);
    bottomPlane = Plane(glm::cross(frontMultFar + cam.cameraUp * halfVSide, cam.cameraRight), cam.cameraPos);
}
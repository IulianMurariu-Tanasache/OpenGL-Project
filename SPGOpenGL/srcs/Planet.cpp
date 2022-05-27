#include "Planet.h"

void Planet::move(float multiplier)
{
	axisRotAngle += axisRotAngleInc * multiplier;
	orbitAngle += orbitAngleInc * multiplier;
}

glm::highp_mat4 Planet::inclineAxis() {
	return glm::rotate(axisInclineAngle, glm::vec3(0,0,1));
}

glm::highp_mat4 Planet::rotateAroundAxis() {
	return glm::rotate(axisRotAngle, glm::vec3(0, 1, 0));
}

glm::highp_mat4 Planet::rotateAroundOrbit() {
	return glm::rotate(orbitAngle, glm::vec3(0, 1, 0));
}

glm::highp_mat4 Planet::moveOnOrbit(float multiplier) {
	return glm::translate(glm::vec3(orbitDist * multiplier, 0, 0));
}
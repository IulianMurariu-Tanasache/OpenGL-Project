#include "Object.h"

Object::Object(FlyweightObjectComponent component) 
{
	reset();
	visible = true;
	baseData = std::make_shared<FlyweightObjectComponent>(component);
}


glm::mat4 Object::getModelMatrix()
{
	modelMatrix = glm::mat4();
	modelMatrix *= glm::translate(positionVec3);
	
	modelMatrix *= glm::rotate(rotationVec3.x, glm::vec3(1, 0, 0));
	modelMatrix *= glm::rotate(rotationVec3.y, glm::vec3(0, 1, 0));
	modelMatrix *= glm::rotate(rotationVec3.z, glm::vec3(0, 0, 1));

	modelMatrix *= glm::scale(scaleVec3);
	return modelMatrix;
}

void Object::scale(glm::vec3 coord)
{
	scaleVec3 *= coord;
}

void Object::rotate(float angle, glm::vec3 axis)
{
	rotationVec3 += angle * axis;
}

void Object::translate(glm::vec3 coord)
{
	positionVec3 += coord;
}

void Object::reset()
{
	scaleVec3 = glm::vec3(1, 1, 1);
	rotationVec3 = glm::vec3(0, 0, 0);
	positionVec3 = glm::vec3(0, 0, 0);
}
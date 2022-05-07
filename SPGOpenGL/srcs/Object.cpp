#include "Object.h"

Object::Object(FlyweightObjectComponent* component, Texture* texture)
{
	//reset();
	visible = true;
	baseData = component;
	this->texture = texture;
	scale = glm::vec3(1, 1, 1);
}
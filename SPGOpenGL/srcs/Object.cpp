#include "Object.h"

Object::Object(unsigned int id, std::shared_ptr<FlyweightObjectComponent> component, std::shared_ptr<Texture> texture, std::shared_ptr<VAOObject> vaoObj)
{
	this->id = id;
	baseData = component;
	this->texture = texture;
	this->vaoObj = vaoObj;
	scale = glm::vec3(1, 1, 1);
}

void Object::drawObject()
{
	texture->bind();
	baseData->bind();
	vaoObj->bind();
	glDrawArrays(GL_TRIANGLES, 0, baseData->getDataSize() / sizeof(Vertex));
}

const FlyweightObjectComponent& Object::getBaseData()
{
	return *baseData.get();
}
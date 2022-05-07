#pragma once
#include "FlyweightObjectComponent.h"
#include "Texture.h"
#include <glm/gtx/transform.hpp>

class Object {
	public:
		FlyweightObjectComponent* baseData;
		Texture* texture;
		bool visible;
		glm::vec3 scale;

		Object(FlyweightObjectComponent* component, Texture* texture);
		FlyweightObjectComponent* getBaseData();
		virtual void move() {};
};

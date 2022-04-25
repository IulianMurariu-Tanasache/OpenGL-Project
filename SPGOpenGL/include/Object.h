#pragma once
#include "FlyweightObjectComponent.h"
#include <glm/gtx/transform.hpp>

class Object {
	public:
		FlyweightObjectComponent* baseData;
		bool visible;

		Object(FlyweightObjectComponent* component);
		FlyweightObjectComponent* getBaseData();
		virtual void move() {};
};

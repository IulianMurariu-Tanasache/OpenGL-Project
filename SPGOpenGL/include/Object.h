#pragma once
#include "FlyweightObjectComponent.h"
#include "Texture.h"
#include "VAOObject.h"
#include <glm/gtx/transform.hpp>
#include <memory>

class Object {
	private:
		std::shared_ptr<Texture> texture;
		std::shared_ptr<VAOObject> vaoObj;
		std::shared_ptr<FlyweightObjectComponent> baseData;

	public:
		glm::vec3 scale;

		Object(std::shared_ptr<FlyweightObjectComponent> component, std::shared_ptr<Texture> texture, std::shared_ptr<VAOObject> vaoObj);
		const FlyweightObjectComponent& getBaseData();
		virtual void move() {};
		virtual void drawObject();
};

#pragma once
#include "FlyweightObjectComponent.h"
#include <memory>
#include <glm/gtx/transform.hpp>

class Object {
	protected:
		std::shared_ptr<FlyweightObjectComponent> baseData;

	public:
		bool visible;
		glm::mat4 modelMatrix;
		glm::vec3 positionVec3;
		glm::vec3 rotationVec3;
		glm::vec3 scaleVec3;

		glm::mat4 getModelMatrix();

		Object(FlyweightObjectComponent component);
		std::shared_ptr<FlyweightObjectComponent> shareBaseData();
		void scale(glm::vec3 coord);
		void rotate(float angle, glm::vec3 axis);
		void translate(glm::vec3 coord);
		void reset();
		virtual void move() {};

};

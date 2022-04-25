#pragma once
#include <glm/gtc/type_ptr.hpp>

enum Direction {
	FORWARDS,BACKWARDS,UP,DOWN,LEFT,RIGHT
};

class Camera {
	private:
		const float moveSpeed = 0.3f;
		const float rotationSpeed = 0.3f;
		glm::mat4 projectionMatrix, viewMatrix;
		glm::vec3 initPos;
		//float xv = 10, yv = 12, zv = 30; //originea sistemului de observare

	public:
		glm::vec3 positionVec3;
		glm::vec3 rotationVec3;
		//glm::vec3 scaleVec3;
		float scaleFactor = 1;

		Camera(int w, int h, glm::vec3 initPos);
		void move(Direction dir);
		void rotate(Direction dir);
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
};
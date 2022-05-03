#pragma once
#include "Frustrum.h"
#define PI glm::pi<float>()

enum Direction {
	FORWARDS,BACKWARDS,UP,DOWN,LEFT,RIGHT
};

class Camera {
	private:
		const float moveSpeed = 40.0f;
		const float rotationSpeed = 50.0f;
		//float xv = 10, yv = 12, zv = 30; //originea sistemului de observare

	public:
		glm::mat4 projectionMatrix, viewMatrix;
		glm::vec3 cameraUp;
		glm::vec3 cameraFront;
		glm::vec3 cameraPos;
		glm::vec3 cameraRight;
		glm::vec3 rotationVec3;
		float fov, aspect, zNear, zFar;
		Frustrum* frustrum = nullptr;
		//glm::vec3 scaleVec3;
		//float scaleFactor = 1;

		Camera(int w, int h, glm::vec3 initPos, float fov = PI / 4, float zNear = 0.1f, float zFar = 100.0f);
		~Camera();
		void move(Direction dir, int deltaTime);
		void rotate(Direction dir, int deltaTime);
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
};
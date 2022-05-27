#pragma once
#include "Frustrum.h"
#define PI glm::pi<float>()

enum Direction {
	FORWARDS,BACKWARDS,UP,DOWN,LEFT,RIGHT
};

class Camera {
	private:
		const float moveSpeed = 42.0f;
		const float rotationSpeed = 0.17f;
		float fov, aspect, zNear, zFar;
		glm::vec3 rotationVec3;
		void calcDirection();

	public:
		glm::mat4 projectionMatrix, viewMatrix;
		glm::vec3 cameraUp;
		glm::vec3 cameraFront;
		glm::vec3 cameraPos;
		glm::vec3 cameraRight;
		Frustrum* frustrum = nullptr;

		Camera(int w, int h, glm::vec3 initPos, float initPitch, float fov = PI / 4, float zNear = 0.1f, float zFar = 400.0f);
		~Camera();
		void move(Direction dir, int deltaTime);
		void rotate(Direction dir, int offset);
		void rotate(Direction dir, float angle);
		void calcFrustrum();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
};
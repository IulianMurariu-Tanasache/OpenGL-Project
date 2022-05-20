#pragma once
#include "Object.h"

class QuadObject {
	private:
		float quadVertices[20] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		GLuint vaoObj;
		GLuint vbo;

	public:
		QuadObject();
		~QuadObject();
		void draw();
};
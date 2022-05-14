#pragma once
#include "Vertex.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "BoundingSphere.h"

class FlyweightObjectComponent {
	private:
		GLuint vbo;
		std::vector< float > completeData;
		std::vector< Vertex > vertexVec; 

	public:
		BoundingSphere* baseVolume = nullptr; //shared_ptr?

		~FlyweightObjectComponent();
		void loadOBJFile(const char* fileName);
		std::size_t getDataSize();
		void bind();
};	

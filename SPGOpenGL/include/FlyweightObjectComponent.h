#pragma once
#include "Vertex.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "BoundingSphere.h"

class FlyweightObjectComponent {
	public:
		//int indexInBuffer;
		std::vector< Vertex > vertexVec;
		std::vector< float > completeData;
		GLuint vbo;
		Volume* baseVolume;

		FlyweightObjectComponent();
		//FlyweightObjectComponent(int index) : FlyweightObjectComponent() { indexInBuffer = index; }
		void loadOBJFile(const char* fileName);
		std::size_t getDataSize();
};	

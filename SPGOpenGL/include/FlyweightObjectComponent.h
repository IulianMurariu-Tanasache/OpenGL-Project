#pragma once
#include "Vertex.h"

class FlyweightObjectComponent {
	public:
		int indexInBuffer;
		std::vector< Vertex > vertexVec;
		std::vector< float > completeData;

		FlyweightObjectComponent() {}
		FlyweightObjectComponent(int index) : indexInBuffer(index) {}
		void loadOBJFile(const char* fileName);
		std::size_t getDataSize();
};	

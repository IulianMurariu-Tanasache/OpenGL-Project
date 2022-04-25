#pragma once
#include "Attribute.h"

class FlyweightObjectComponent {
	public:
		Attribute vertices;
		Attribute normals;
		Attribute uvs;
		Attribute colors;
		std::vector< glm::vec3 > completeData;
		FlyweightObjectComponent() {};
		void loadOBJFile(const char* fileName);
		std::size_t getDataSize();
};	

#include "FlyweightObjectComponent.h"
#include "objloader.hpp"

void FlyweightObjectComponent::loadOBJFile(const char* fileName)
{
	//sa fac ceva cu uv-urile astea?
	std::vector < glm::vec2 > _uvs;
	bool res = loadOBJ(fileName, vertices.data, _uvs, normals.data);
	//tratare erori?

	completeData = vertices.data;
	completeData.insert(completeData.end(), normals.data.begin(), normals.data.end());
}

std::size_t FlyweightObjectComponent::getDataSize()
{
	return completeData.size() * sizeof(float) * 3;
}
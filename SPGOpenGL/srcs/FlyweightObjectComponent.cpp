#include "FlyweightObjectComponent.h"
#include "objloader.hpp"
#include <iostream>

void FlyweightObjectComponent::loadOBJFile(const char* fileName)
{
	//sa fac ceva cu uv-urile astea?
	std::vector < glm::vec2 > _uvs;
	std::vector < glm::vec3 > _normals;
	std::vector < glm::vec3 > _vertices;
	bool res = loadOBJ(fileName, _vertices, _uvs, _normals);
	//tratare erori?

	//varianta una dupa alta
	/*completeData = vertices.data;
	completeData.insert(completeData.end(), normals.data.begin(), normals.data.end());*/

	//varianta atribute intercalate
	for (int i = 0; i < _vertices.size(); ++i)
	{
		this->completeData.push_back(_vertices.at(i).x);
		this->completeData.push_back(_vertices.at(i).y);
		this->completeData.push_back(_vertices.at(i).z);
		this->completeData.push_back(_normals.at(i).x);
		this->completeData.push_back(_normals.at(i).y);
		this->completeData.push_back(_normals.at(i).z);
		Vertex v = Vertex();
		v.position = _vertices.at(i);
		v.normals = _normals.at(i);
		//v.uvs = _uvs.at(i);
		this->vertexVec.push_back(v);
	}
}


std::size_t FlyweightObjectComponent::getDataSize()
{
	return vertexVec.size() * sizeof(Vertex);
}
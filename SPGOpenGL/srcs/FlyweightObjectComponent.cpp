#include "FlyweightObjectComponent.h"
#include "objloader.hpp"
#include <iostream>
#include <algorithm>


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

	//calcul centru si scale
	glm::vec3 minVec = glm::vec3(_vertices.at(0));
	glm::vec3 maxVec = glm::vec3(_vertices.at(0));

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

		if (_vertices.at(i).x > maxVec.x) maxVec.x = _vertices.at(i).x;
		if (_vertices.at(i).y > maxVec.y) maxVec.y = _vertices.at(i).y;
		if (_vertices.at(i).z > maxVec.z) maxVec.z = _vertices.at(i).z;

		if (_vertices.at(i).x < minVec.x) minVec.x = _vertices.at(i).x;
		if (_vertices.at(i).y < minVec.y) minVec.y = _vertices.at(i).y;
		if (_vertices.at(i).z < minVec.z) minVec.z = _vertices.at(i).z;
	}

	glm::vec3 center = glm::vec3((maxVec.x + minVec.x) / 2.0f, (maxVec.y + minVec.y) / 2.0f, (maxVec.z + minVec.z) / 2.0f);
	float radius = std::max(	std::max(abs(center.x - maxVec.x), abs(center.x - minVec.x)),
								std::max(	std::max(abs(center.y - maxVec.y), abs(center.y - minVec.y)),
											std::max(abs(center.z - maxVec.z), abs(center.z - minVec.z))));
	baseVolume = new BoundingSphere(center, radius);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, this->getDataSize(), (void*)this->completeData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

FlyweightObjectComponent::~FlyweightObjectComponent()
{
	delete baseVolume;
	baseVolume = nullptr;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
}


std::size_t FlyweightObjectComponent::getDataSize()
{
	return vertexVec.size() * sizeof(Vertex);
}
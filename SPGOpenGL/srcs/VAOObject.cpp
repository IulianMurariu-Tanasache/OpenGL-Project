#include "VAOObject.h"
#include "Vertex.h"
#include <utility>

VAOObject::VAOObject()
{
	glGenVertexArrays(1, &vaoObj);

	attributeMap.insert(std::pair<const char*, std::size_t>("position", 0));
	attributeMap.insert(std::pair<const char*, std::size_t>("normals", sizeof(Vertex::position)));
	//VAOObject::attributeMap.insert(std::pair<const char*, std::size_t>("color", attributeMap.at("normals") + sizeof((Vertex::normals)));
	//VAOObject::attributeMap.insert(std::pair<const char*, std::size_t>("uvs", attributeMap.at("color") + sizeof((Vertex::color)));
}


std::size_t VAOObject::stride() //distance between consecutive elements of same attribute
{
	return sizeof(Vertex);
}

std::size_t VAOObject::offsettOf(const char* attribute) //where is the first element of this attibute
{
	return attributeMap.at(attribute);
}

void VAOObject::bind()
{
	glBindVertexArray(vaoObj);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride(), (void*)offsettOf("position"));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride(), (void*)offsettOf("normals"));
}
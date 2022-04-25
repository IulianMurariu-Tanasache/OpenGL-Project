#include "Vertex.h"
#include <utility>

std::map<const char*, std::size_t> Vertex::attributeMap;
void Vertex::init()
{
	Vertex::attributeMap.insert(std::pair<const char*, std::size_t>("position", 0));
	Vertex::attributeMap.insert(std::pair<const char*, std::size_t>("normals", sizeof(position)));
	Vertex::attributeMap.insert(std::pair<const char*, std::size_t>("color", attributeMap.at("normals") + sizeof(normals)));
	Vertex::attributeMap.insert(std::pair<const char*, std::size_t>("uvs", attributeMap.at("color") + sizeof(normals)));
}

std::size_t Vertex::stride() //distance between consecutive elements of same attribute
{
	return sizeof(Vertex);
}

std::size_t Vertex::offsettOf(const char* attribute) //where is the first element of this attibute
{
	return Vertex::attributeMap.at(attribute);
}
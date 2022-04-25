#pragma once
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <map>

class Vertex {
	public:
		glm::vec3 position;
		//glm::vec4 color;
		//glm::vec2 uvs;
		glm::vec3 normals;
		static std::map<const char*, std::size_t> attributeMap;

		static void init();
		static std::size_t stride(); //distance between consecutive elements of same attribute
		static std::size_t offsettOf(const char* attribute); //where is the first element of this attibute
};
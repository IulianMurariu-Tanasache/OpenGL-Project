#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <GL/glew.h>
#include <GL/freeglut.h>

class VAOObject {
	public:
		std::map<const char*, std::size_t> attributeMap;
		GLuint vaoObj;

		VAOObject();
		~VAOObject();
		std::size_t stride();
		std::size_t offsettOf(const char* attribute);
		virtual void bind();
};

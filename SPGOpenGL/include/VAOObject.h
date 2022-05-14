#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <GL/glew.h>
#include <GL/freeglut.h>

class VAOObject {
	protected:
		std::map<const char*, std::size_t> attributeMap;
		GLuint vaoObj;

	public:
		VAOObject();
		~VAOObject();
		virtual std::size_t stride();
		virtual std::size_t offsettOf(const char* attribute);
		virtual void bind();
};

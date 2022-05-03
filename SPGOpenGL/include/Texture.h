#pragma once
#include <string>
//#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Texture
{
	private:
		std::string filePath;
		unsigned char* localBuffer;
		GLuint textureObj;
	public:
		int w, h, nrChannels;

		Texture(const std::string& path, int flip = 0);
		~Texture();
		void bind(unsigned int slot = 0) const;
		void unbind() const;

};
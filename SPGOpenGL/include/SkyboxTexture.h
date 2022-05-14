#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "VAOObject.h"
#include "FlyweightObjectComponent.h"

class SkyboxTexture
{
	private:
		std::vector< char* > facePaths;
		GLuint textureObj;
		int w, h, nrChannels;

	public:
		SkyboxTexture(std::vector<std::string>& faces, int flip = 0);
		~SkyboxTexture();
		void bind(unsigned int slot = 0) const;
		void unbind() const;
};

#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path, int flip)
	:filePath(path), localBuffer(nullptr), w(0), h(0), nrChannels(0)
{
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//load texture !!! bottom-left e 0,0 pt texturi !!!
	stbi_set_flip_vertically_on_load(flip);
	localBuffer = stbi_load(filePath.c_str(), &w, &h, &nrChannels, 0);

	if (localBuffer)
	{
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer);
		}
		else if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(localBuffer);
		std::cout << "Loaded texture from " << filePath << '\n';
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureObj);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureObj);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
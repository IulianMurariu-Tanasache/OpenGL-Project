#include "SkyboxTexture.h"
#include "stb_image.h"
#include <iostream>

SkyboxTexture::SkyboxTexture(std::vector<std::string>& faces, int flip)
{
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureObj);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            //check for nrChannels
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
            std::cout << "Loaded texture from " << faces.at(i) << '\n';
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces.at(i) << std::endl;
            stbi_image_free(data);
        }
    }

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

SkyboxTexture::~SkyboxTexture()
{
    glDeleteTextures(1, &textureObj);
}

void SkyboxTexture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureObj);
}

void SkyboxTexture::unbind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

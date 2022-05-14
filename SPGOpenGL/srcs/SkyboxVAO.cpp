#include "SkyboxVAO.h"

SkyboxVAO::SkyboxVAO(GLuint vbo)
{
	glGenVertexArrays(1, &vaoObj);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	bind();
	attributeMap.insert(std::pair<const char*, std::size_t>("position", 0));
}

SkyboxVAO::~SkyboxVAO()
{
	attributeMap.clear();

	glBindVertexArray(0);
	glDeleteBuffers(1, &vaoObj);
}

std::size_t SkyboxVAO::stride()
{
	return 3 * sizeof(float);
}

std::size_t SkyboxVAO::offsettOf(const char* attribute)
{
	return attributeMap.at(attribute);
}

void SkyboxVAO::bind()
{
	glBindVertexArray(vaoObj);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride(), NULL);
}
#include "QuadObject.h"

QuadObject::QuadObject() 
{
	glGenVertexArrays(1, &vaoObj);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vaoObj);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void QuadObject::draw()
{
	glBindVertexArray(vaoObj);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

QuadObject::~QuadObject()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vaoObj);
}
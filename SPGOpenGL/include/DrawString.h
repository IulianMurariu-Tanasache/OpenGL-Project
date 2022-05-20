#pragma once
#include "GL/freeglut.h"
#include <GL/glew.h>
#include <string>

void drawString(std::string text, unsigned int x, unsigned int y, unsigned int width)
{
	glUseProgram(0);
	int w = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text.c_str());
	if (w > width - x)
	{
		x = width - w - 5;
	}
	glWindowPos2i(x, y);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text.c_str());
	glEnable(GL_LIGHTING);
}
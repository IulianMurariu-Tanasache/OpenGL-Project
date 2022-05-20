#pragma once
#include "GL/freeglut.h"
#include <GL/glew.h>
#include <string>

void drawString(std::string text, unsigned int x, unsigned int y)
{
	glColor3f(1.0, 1.0, 1.0);
	glWindowPos2i(10, 10);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text.c_str());
}
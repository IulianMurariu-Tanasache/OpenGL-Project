﻿#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include "Planet.h"
#include "Camera.h"
#include "ShaderManager.h"
#include <stack>

#define PI glm::pi<float>()

Camera* camera;
std::stack<glm::mat4> modelStack;
Planet* planetObject;
ShaderManager* shaderManager;
FlyweightObjectComponent sphereComp;
GLuint vaoObj, vboObj;
glm::mat4 modelMatrix;

glm::vec3 lightPos(0, 20000, 0);
glm::vec3 viewPos(2, 3, 6);

float axisRotAngle = PI / 16.0; // unghiul de rotatie in jurul propriei axe
float radius = 2;
float scaleFactor = 0.1;

void init()
{

	sphereComp = FlyweightObjectComponent();
	sphereComp.loadOBJFile("obj/sphere.obj");

	camera = new Camera(1024,720,glm::vec3(10,12,30));

	planetObject = new Planet(sphereComp, 1, 0, PI / 16, 8.0, PI / 8, PI / 32);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0);

	glewInit();

	glGenBuffers(1, &vboObj);
	glBindBuffer(GL_ARRAY_BUFFER, vboObj);
	glBufferData(GL_ARRAY_BUFFER, sphereComp.getDataSize(), (void*)sphereComp.completeData.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoObj);
	glBindVertexArray(vaoObj);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sphereComp.vertices.data.size() * sizeof(float) * 3));

	shaderManager = new ShaderManager();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderManager->getShaderProgramme());

	glBindVertexArray(vaoObj);

	GLuint lightPosLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

	GLuint viewPosLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

	modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate
	//planetObject->move();
	modelMatrix *= planetObject->rotateAroundOrbit();
	modelMatrix *= planetObject->moveOnOrbit();
	modelMatrix *= planetObject->rotateAroundAxis();
	modelMatrix *= glm::scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	GLuint modelMatrixLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "modelViewProjectionMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix));

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	GLuint normalMatrixLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "normalMatrix");
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glDrawArrays(GL_TRIANGLES, 0, sphereComp.vertices.data.size());

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		axisRotAngle += 0.1;
		if (axisRotAngle > 2 * PI) 
			axisRotAngle = 0;

		break;
	case 'e':

		axisRotAngle -= 0.1;
		if (axisRotAngle < 0)
			axisRotAngle = 2 * PI;
		break;

	case 'w':
		camera->move(FORWARDS);
		break;
	case 's':
		camera->move(BACKWARDS);
		break;
	case 'z':
		camera->rotate(LEFT);
		break;
	case 'x':
		camera->rotate(RIGHT);
		break;
	case 'a':
		camera->move(LEFT);
		break;
	case 'd':
		camera->move(RIGHT);
		break;
	case '+':
		scaleFactor += 0.01;
		break;
	case '-':
		scaleFactor -= 0.01;
	};
	glutPostRedisplay(); // cauzeaza redesenarea ferestrei
}

//void My_mouse_routine(int x, int y)
//{
//
//	directieX = (x - 350) / 500.0f;// place current mouse pos in mouse_x
//	directieY= (y - 350) / 500.0f;
//	glutPostRedisplay();
//}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 200);
	glutInitWindowSize(1024, 720);
	glutCreateWindow("Cel mai engine grafic");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//glutPassiveMotionFunc(My_mouse_routine);
	glutMainLoop();

	return 0;
}

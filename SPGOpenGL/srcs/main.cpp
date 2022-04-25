#include <iostream>
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
#include <time.h>

#define PI glm::pi<float>()
#define FPS 120

/*
	Poate fac batch rendering calumea candva, sau alte optimizari in loc de VBO pentru fiecare obiet
*/


Camera* camera;
std::vector< float > allObjectsData;
std::stack<glm::mat4> modelStack;
Planet* planetObject;
ShaderManager* shaderManager;
FlyweightObjectComponent sphereComp;
FlyweightObjectComponent scratComp;
GLuint vaoObj, vboObj;
glm::mat4 modelMatrix;
int frame_count = 0;
int start_time,final_time;

glm::vec3 lightPos(0, 20000, 0);
glm::vec3 viewPos(2, 3, 6);

float axisRotAngle = PI / 16.0; // unghiul de rotatie in jurul propriei axe
float radius = 2;
float scaleFactor = 0.1;

void init()
{
	Vertex::init();
	//std::cout << "Size of glm::vec3: " << sizeof(glm::vec3) << '\n';
	//std::cout << "Size of float[3]: " << sizeof(float[3]) << '\n';
	sphereComp = FlyweightObjectComponent(0);
	sphereComp.loadOBJFile("obj/sphere.obj");

	scratComp = FlyweightObjectComponent(1);
	scratComp.loadOBJFile("obj/scrat.obj");

	camera = new Camera(1024,720,glm::vec3(10,12,30));

	planetObject = new Planet(sphereComp, 0, PI / 16, 8.0, PI / 8, PI / 32);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0);

	glewInit();

	allObjectsData = sphereComp.completeData;
	allObjectsData.insert(allObjectsData.end(), scratComp.completeData.begin(), scratComp.completeData.end());

	glGenBuffers(1, &vboObj);
	glBindBuffer(GL_ARRAY_BUFFER, vboObj);
	glBufferData(GL_ARRAY_BUFFER, sphereComp.getDataSize() + scratComp.getDataSize(), allObjectsData.data(), GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vaoObj);
	glBindVertexArray(vaoObj);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::stride(), (void*)Vertex::offsettOf("position"));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::stride(), (void*)Vertex::offsettOf("normals"));

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
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

	GLuint modelMatrixLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "modelViewProjectionMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix));

	GLuint normalMatrixLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "normalMatrix");
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, vboObj);
	glDrawArrays(GL_TRIANGLES, 0, sphereComp.vertexVec.size());

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
	if( key == 'w'){
		camera->move(DOWN);
	}
	if( key == 's'){
		camera->move(UP);
	}
	if( key == 'z'){
		camera->rotate(LEFT);
	}
	if( key == 'x'){
		camera->rotate(RIGHT);
	}
	if( key == 'a'){
		camera->move(LEFT);
	}
	if( key == 'd'){
		camera->move(RIGHT);
	}
	if( key == '+'){
		scaleFactor += 0.01;
	}
	if( key == '-'){
		scaleFactor -= 0.01;
	}
	//glutPostRedisplay(); // cauzeaza redesenarea ferestrei
}

//void My_mouse_routine(int x, int y)
//{
//
//	directieX = (x - 350) / 500.0f;// place current mouse pos in mouse_x
//	directieY= (y - 350) / 500.0f;
//	glutPostRedisplay();
//}

void frameFunc(int) {
	frame_count++;
	final_time = time(NULL);
	if (final_time - start_time > 0) {
		std::cout << "FPS: " << frame_count << std::endl;
		frame_count = 0;
		start_time = final_time;
	}
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, frameFunc, 0);
}

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
	start_time = time(NULL);
	glutTimerFunc(1000 / FPS, frameFunc, 0);
	//glutPassiveMotionFunc(My_mouse_routine);
	glutMainLoop();

	return 0;
}

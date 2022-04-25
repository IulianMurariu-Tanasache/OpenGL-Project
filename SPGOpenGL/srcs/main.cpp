#include "Planet.h"
#include "Camera.h"
#include "ShaderManager.h"
#include <stack>
#include <time.h>
#include <iostream>

#define PI glm::pi<float>()
#define FPS 120

/*
	Poate fac batch rendering calumea candva, sau alte optimizari in loc de VBO pentru fiecare obiet
*/

FlyweightObjectComponent* sphereComp;
FlyweightObjectComponent* scratComp;

Camera* camera;
Planet* planetObject;
Object* scratObject;
ShaderManager* shaderManager;

GLuint vaoObj;
glm::mat4 modelMatrix;
std::stack<glm::mat4> modelStack;

int frame_count = 0;
int start_time,final_time;

glm::vec3 lightPos(0, 20000, 0);
glm::vec3 viewPos(2, 3, 6);

float scaleFactor = 0.1;

void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0);
	glewInit();

	Vertex::init();

	sphereComp = new FlyweightObjectComponent();
	scratComp = new FlyweightObjectComponent();

	sphereComp->loadOBJFile("obj/sphere.obj");
	scratComp->loadOBJFile("obj/scrat.obj");

	camera = new Camera(1024,720,glm::vec3(10,12,30));
	planetObject = new Planet(sphereComp, 0, PI / 128, 3.0, PI / 8, PI / 128);
	scratObject = new Object(scratComp);
	shaderManager = new ShaderManager();

	glGenVertexArrays(1, &vaoObj);
}

void setUpVAO()
{
	glBindVertexArray(vaoObj);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::stride(), (void*)Vertex::offsettOf("position"));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::stride(), (void*)Vertex::offsettOf("normals"));
}

void drawObject(FlyweightObjectComponent* comp)
{
	glBindBuffer(GL_ARRAY_BUFFER, comp->vbo);
	setUpVAO();
	glDrawArrays(GL_TRIANGLES, 0, comp->vertexVec.size());
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderManager->getShaderProgramme());

	GLuint lightPosLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

	GLuint viewPosLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

	modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate
	planetObject->move();
	modelMatrix *= glm::translate(glm::vec3(0, 1, 0));
	modelMatrix *= planetObject->rotateAroundOrbit();
	modelMatrix *= planetObject->moveOnOrbit();
	modelMatrix *= planetObject->rotateAroundAxis();
	modelMatrix *= glm::scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

	GLuint modelMatrixLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "modelViewProjectionMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix));

	GLuint normalMatrixLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "normalMatrix");
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	drawObject(planetObject->baseData);

	modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate
	modelMatrix *= glm::rotate(PI / 16.0f, glm::vec3(0, 1, 0));
	modelMatrix *= glm::scale(glm::vec3(scaleFactor / 10.0f, scaleFactor / 10.0f, scaleFactor / 10.0f));
	normalMatrix = glm::transpose(glm::inverse(modelMatrix));

	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix));
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	drawObject(scratObject->baseData);

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
		//scaleFactor += 0.01;
		camera->move(FORWARDS);
	}
	if( key == '-'){
		//scaleFactor -= 0.01;
		camera->move(BACKWARDS);
	}
}

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

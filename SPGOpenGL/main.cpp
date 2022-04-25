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
#include <stack>

#define PI glm::pi<float>()

GLuint shader_programme;
Camera* camera;
std::stack<glm::mat4> modelStack;
Planet* planetObject;
FlyweightObjectComponent sphereComp;
GLuint vaoObj, vboObj;
glm::mat4 modelMatrix;

glm::vec3 lightPos(0, 20000, 0);
glm::vec3 viewPos(2, 3, 6);

float axisRotAngle = PI / 16.0; // unghiul de rotatie in jurul propriei axe
float radius = 2;
float scaleFactor = 0.1;

void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

std::string textFileRead(char *fn)
{
	std::ifstream ifile(fn);
	std::string filetext;
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		filetext.append(line + "\n");
	}
	return filetext;
}

void init()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//bool res = loadOBJ("obj/scrat.obj", vertices, uvs, normals);

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

	std::string vstext = textFileRead("vertex.vert");
	std::string fstext = textFileRead("fragment.frag");
	const char* vertex_shader = vstext.c_str();
	const char* fragment_shader = fstext.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	// check for shader compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	printShaderInfoLog(fs);
	printShaderInfoLog(vs);
	printProgramInfoLog(shader_programme);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_programme);

	glBindVertexArray(vaoObj);

	GLuint lightPosLoc = glGetUniformLocation(shader_programme, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

	GLuint viewPosLoc = glGetUniformLocation(shader_programme, "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

	modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate
	//planetObject->move();
	modelMatrix *= planetObject->rotateAroundOrbit();
	modelMatrix *= planetObject->moveOnOrbit();
	modelMatrix *= planetObject->rotateAroundAxis();
	modelMatrix *= glm::scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	GLuint modelMatrixLoc = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix));

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
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

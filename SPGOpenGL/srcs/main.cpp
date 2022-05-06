﻿#include "Planet.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "VAOObject.h"
#include "BoundingSphere.h"
#include "Texture.h" 
#include "Skybox.h"
#include <stack>
#include <time.h>
#include <iostream>
#include "stb_image.h"

#define PI glm::pi<float>()
#define FPS 120

/*
	Poate fac batch rendering calumea candva, sau alte optimizari in loc de VBO pentru fiecare obiect
	-skybox
	-culoare
	-zoom, schimband nr de vertexuri?
	-planete si telescop
	-soare, alta sursa de lumina?
	-shared_ptr pt flyweightcomp
	-make vbo and vao more generic
	-shader location for uniforms make initially and store them
*/

FlyweightObjectComponent* sphereComp;
FlyweightObjectComponent* scratComp;
Texture* planetMoonTexture;
VAOObject* vaoObj;

Camera* camera;
Planet* planetObject;
Object* scratObject;
ShaderManager* shaderManager;
Skybox* skybox;

glm::mat4 modelMatrix;
std::stack<glm::mat4> modelStack;

int frame_count = 0;
int start_time,final_time;
int deltaTime = 0.0f;	// Time between current frame and last frame
int lastFrameTime = 0.0f; // Time of last frame

glm::vec3 lightPos(0, 20000, 0);
glm::vec3 viewPos(2, 3, 6);

float scaleFactor = 0.1;
void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0);
	glewInit();

	vaoObj = new VAOObject();

	sphereComp = new FlyweightObjectComponent();
	scratComp = new FlyweightObjectComponent();

	sphereComp->loadOBJFile("obj/sfera-fina.obj");
	scratComp->loadOBJFile("obj/scrat.obj");

	planetMoonTexture = new Texture("textures/planet_moon.jpg");

	camera = new Camera(1024,720,glm::vec3(5,5,20));
	planetObject = new Planet(sphereComp, 0, PI / 128, 3.0, PI / 8, PI / 128);
	scratObject = new Object(scratComp);
	shaderManager = new ShaderManager();

	std::vector<std::string> faces = {
		    "textures/skybox/right.jpg",
			"textures/skybox/left.jpg",
			"textures/skybox/top.jpg",
			"textures/skybox/bottom.jpg",
			"textures/skybox/front.jpg",
			"textures/skybox/back.jpg"
	};
	skybox = new Skybox(faces);
}



void drawObject(FlyweightObjectComponent* comp)
{
	glBindBuffer(GL_ARRAY_BUFFER, comp->vbo);
	vaoObj->bind();
	glDrawArrays(GL_TRIANGLES, 0, comp->vertexVec.size());
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw skybox
	glDepthMask(GL_FALSE);
	glUseProgram(shaderManager->getShaderSkybox());

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::mat4(glm::mat3(glm::lookAt(camera->cameraPos, camera->cameraPos + camera->cameraFront, camera->cameraUp)));

	GLuint projMatrixLoc = glGetUniformLocation(shaderManager->getShaderSkybox(), "projection");
	glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	GLuint viewMatrixLoc = glGetUniformLocation(shaderManager->getShaderSkybox(), "view");
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint skyboxTextureLoc = glGetUniformLocation(shaderManager->getShaderSkybox(), "skybox");
	glUniform1i(skyboxTextureLoc, 0);

	skybox->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//draw rest of the scene
	glDepthMask(GL_TRUE);
	glUseProgram(shaderManager->getShaderProgramme());

	GLuint lightPosLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

	GLuint viewPosLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera->cameraPos));

	GLuint textureLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "currentTexture");
	glUniform1i(textureLoc, 0);

	glm::vec3 scale = { 1,1,1 };

	glDepthMask(GL_TRUE);
	planetMoonTexture->bind();
	modelMatrix = glm::mat4();
	planetObject->move();
	modelMatrix *= glm::translate(glm::vec3(0, 1, 0));
	modelMatrix *= planetObject->rotateAroundOrbit();
	modelMatrix *= planetObject->moveOnOrbit();
	modelMatrix *= planetObject->rotateAroundAxis();
	scale = glm::vec3(scaleFactor, scaleFactor, scaleFactor);
	modelMatrix *= glm::scale(scale);
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

	GLuint modelMatrixLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "modelViewProjectionMatrix");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix));

	GLuint normalMatrixLoc = glGetUniformLocation(shaderManager->getShaderProgramme(), "normalMatrix");
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	if (planetObject->baseData->baseVolume->isOnFrustrum(*camera->frustrum, modelMatrix, scale))
		drawObject(planetObject->baseData);

	modelMatrix = glm::mat4();
	modelMatrix *= glm::rotate(PI / 16.0f, glm::vec3(0, 1, 0));
	scale = glm::vec3(scaleFactor / 10.0f, scaleFactor / 10.0f, scaleFactor / 10.0f);
	modelMatrix *= glm::scale(scale);
	normalMatrix = glm::transpose(glm::inverse(modelMatrix));

	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix));
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	if (scratObject->baseData->baseVolume->isOnFrustrum(*camera->frustrum, modelMatrix, scale))
		drawObject(scratObject->baseData);
		

	//std::cout << "E scratch visible? " << scratObject->baseData->baseVolume->isOnFrustrum(*camera->frustrum, modelMatrix, scale) << '\n';

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
	if( key == 'w'){
		camera->move(DOWN, deltaTime);
	}
	if( key == 's'){
		camera->move(UP, deltaTime);
	}
	if( key == 'a'){
		camera->move(LEFT, deltaTime);
	}
	if( key == 'd'){
		camera->move(RIGHT, deltaTime);
	}
	if( key == 'z'){
		camera->rotate(LEFT, deltaTime);
	}
	if( key == 'x'){
		camera->rotate(RIGHT, deltaTime);
	}
	if( key == '+'){
		//scaleFactor += 0.01;
		camera->move(FORWARDS, deltaTime);
	}
	if( key == '-'){
		//scaleFactor -= 0.01;
		camera->move(BACKWARDS, deltaTime);
	}
}

void frameFunc(int) {
	frame_count++;
	int currentFrame = glutGet(GLUT_ELAPSED_TIME); 
	deltaTime = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;

	final_time = time(NULL);
	if (final_time - start_time > 0) {
		//std::cout << "FPS: " << frame_count << std::endl;
		frame_count = 0;
		start_time = final_time;
	}
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, frameFunc, 0);
}

void onExit()
{
	delete sphereComp;
	delete scratComp;
	delete planetMoonTexture;
	delete vaoObj;
	delete camera;
	delete planetObject;
	delete scratObject;
	delete shaderManager;
	delete skybox;
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
	atexit(onExit);
	//glutPassiveMotionFunc(My_mouse_routine);
	glutMainLoop();

	return 0;
}

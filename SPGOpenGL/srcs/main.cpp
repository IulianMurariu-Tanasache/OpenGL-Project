#include "Planet.h"
#include "Camera.h"
#include "Shader.h"
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
#define AU 9.0f//astronomical unit
#define ORBIT_SPEED PI / 2048 
#define EARTH_AXIS_ROTATION PI / 80
#define SUN_OFFSET 6.0f

/*
	-zoom, schimband nr de vertexuri???
	-soare, alta sursa de lumina? - BLOOM
	-make vbo and vao more generic
	-cratere pe planete - Normal mapping?
	-eclipse - umbre
	-adaugat inclinatie la planete
	-calcul modelviewprojection matrix in shader + normalMatrix tot in shadeR?
*/

std::vector<Planet> planets;
std::unique_ptr<Camera> camera;
std::unique_ptr<Shader> planetShader;
std::unique_ptr<Shader> skyboxShader;
std::unique_ptr<Skybox> skybox;

glm::mat4 modelMatrix;
std::stack<glm::mat4> modelStack;

int frame_count = 0;
int start_time,final_time;
int deltaTime = 0;	// Time between current frame and last frame
int lastFrameTime = 0; // Time of last frame

glm::vec3 lightPos(0, 20000, 0);

#define EARTH_SCALE 0.5f

void allocPlanets()
{
	//load obj
	std::shared_ptr<FlyweightObjectComponent> sphereComp = std::make_shared<FlyweightObjectComponent>();
	sphereComp->loadOBJFile("obj/sfera-fina.obj");

	//load textures
	std::shared_ptr<Texture> planetMoonTexture = std::make_shared<Texture>("textures/planet_moon.jpg");
	std::shared_ptr<Texture> sunTexture = std::make_shared<Texture>("textures/sun.jpg");
	std::shared_ptr<Texture> mercuryTexture = std::make_shared<Texture>("textures/mercury.jpg");
	std::shared_ptr<Texture> venusTexture = std::make_shared<Texture>("textures/venus.jpg");
	std::shared_ptr<Texture> earthTexture = std::make_shared<Texture>("textures/earth.jpg");
	std::shared_ptr<Texture> marsTexture = std::make_shared<Texture>("textures/mars.jpg");
	std::shared_ptr<Texture> jupiterTexture = std::make_shared<Texture>("textures/jupiter.jpg");
	std::shared_ptr<Texture> saturnTexture = std::make_shared<Texture>("textures/saturn.jpg");
	std::shared_ptr<Texture> uranusTexture = std::make_shared<Texture>("textures/uranus.jpg");
	std::shared_ptr<Texture> neptuneTexture = std::make_shared<Texture>("textures/neptune.jpg");

	//create VAO
	std::shared_ptr<VAOObject> vaoObj = std::make_shared<VAOObject>();

	//std::shared_ptr<FlyweightObjectComponent> component, std::shared_ptr<Texture> texture, std::shared_ptr<VAOObject> vaoObj,
	//float _axisRotAngle, float _axisRotAngleInc, float _orbitDist, float _orbitAngle, float _orbitAngleInc
	planets.push_back(Planet(sphereComp, sunTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 200.0f, 0, 0, 0));
	planets.back().scale = glm::vec3(14.0f * EARTH_SCALE, 14.0f * EARTH_SCALE, 14.0f * EARTH_SCALE);
	planets.push_back(Planet(sphereComp, mercuryTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 40.0f, 0.4f * AU + SUN_OFFSET, 0, 47.0f * ORBIT_SPEED)); //mercury
	planets.back().scale = glm::vec3((1 / 2.0f) * EARTH_SCALE, (1 / 2.0f) * EARTH_SCALE, (1 / 2.0f) * EARTH_SCALE);
	planets.push_back(Planet(sphereComp, venusTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 100.0f, 0.7f * AU + SUN_OFFSET, 0, 35.02f * ORBIT_SPEED)); //venus
	planets.back().scale = glm::vec3(1 * EARTH_SCALE, 1 * EARTH_SCALE, 1 * EARTH_SCALE);
	planets.push_back(Planet(sphereComp, earthTexture, vaoObj, 0, EARTH_AXIS_ROTATION, AU + SUN_OFFSET, 0, 29.78f * ORBIT_SPEED)); //earth
	planets.back().scale = glm::vec3(1 * EARTH_SCALE, 1 * EARTH_SCALE, 1 * EARTH_SCALE);
	planets.push_back(Planet(sphereComp, marsTexture, vaoObj, 0, EARTH_AXIS_ROTATION, 1.5f * AU + SUN_OFFSET, 0, 26.5f * ORBIT_SPEED)); //mars
	planets.back().scale = glm::vec3((1 / 1.3f) * EARTH_SCALE, (1 / 1.3f) * EARTH_SCALE, (1 / 1.3f) * EARTH_SCALE);
	planets.push_back(Planet(sphereComp, jupiterTexture, vaoObj, 0, EARTH_AXIS_ROTATION * 2.4f, 5.0f * AU + SUN_OFFSET, 0, 13.06f * ORBIT_SPEED)); //jupiter
	planets.back().scale = glm::vec3(4.2f * EARTH_SCALE, 4.2f * EARTH_SCALE, 4.2f * EARTH_SCALE);
	planets.push_back(Planet(sphereComp, saturnTexture, vaoObj, 0, EARTH_AXIS_ROTATION * 2.2f, 7.5f * AU + SUN_OFFSET, 0, 10 * ORBIT_SPEED)); //saturn
	planets.back().scale = glm::vec3(3.35f * EARTH_SCALE, 3.35f * EARTH_SCALE, 3.35f * EARTH_SCALE);
	planets.push_back(Planet(sphereComp, uranusTexture, vaoObj, 0, EARTH_AXIS_ROTATION * 1.4f, 9.8f * AU + SUN_OFFSET, 0, 7.11f * ORBIT_SPEED)); //uranus
	planets.back().scale = glm::vec3(1.6f * EARTH_SCALE, 1.6f * EARTH_SCALE, 1.6f * EARTH_SCALE);
	planets.push_back(Planet(sphereComp, neptuneTexture, vaoObj, 0, EARTH_AXIS_ROTATION * 1.5f, 11.1f * AU + SUN_OFFSET, 0, 5.43f * ORBIT_SPEED)); //neptune
	planets.back().scale = glm::vec3(1.61f * EARTH_SCALE, 1.61f * EARTH_SCALE, 1.61f * EARTH_SCALE);
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0);
	glewInit();

	camera = std::make_unique<Camera>(1024, 720, glm::vec3(0, 60, 70), 40.0f);

	planetShader = std::make_unique<Shader>("shaders/vertex.vert","shaders/fragment.frag");
	//planetShader->setVec3("lightPos", lightPos);
	planetShader->setInt("currentTexture", 0);

	skyboxShader = std::make_unique<Shader>("shaders/vertexSkybox.vert","shaders/fragmentSkybox.frag");
	skyboxShader->setInt("skybox", 0);
	//skyboxShader->setMat4("projection", camera->getProjectionMatrix());

	allocPlanets();

	std::vector<std::string> faces = {
			"textures/skybox/right.png",
			"textures/skybox/left.png",
			"textures/skybox/top.png",
			"textures/skybox/bottom.png",
			"textures/skybox/front.png",
			"textures/skybox/back.png"
	};
	skybox = std::make_unique<Skybox>(faces);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);

	//draw skybox
	glDepthMask(GL_FALSE);
	skyboxShader->use();
	skyboxShader->setMat4("projection", camera->getProjectionMatrix());

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::mat4(glm::mat3(glm::lookAt(camera->cameraPos, camera->cameraPos + camera->cameraFront, camera->cameraUp)));
	skyboxShader->setMat4("view", view);

	skybox->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//draw rest of the scene
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	planetShader->use();
	planetShader->setVec3("viewPos", camera->cameraPos);
	planetShader->setVec3("lightPos", lightPos);

	glm::vec3 scale = { 1,1,1 };

	modelMatrix = glm::mat4();
	for (Planet& planetObject : planets) {
	//Planet& planetObject = planets.at(5);
		planetObject.move();
		modelMatrix = glm::mat4();
		modelMatrix *= glm::translate(glm::vec3(0, 1, 0));
		modelMatrix *= planetObject.rotateAroundOrbit();
		modelMatrix *= planetObject.moveOnOrbit();
		modelMatrix *= planetObject.rotateAroundAxis();
		scale = planetObject.scale;
		modelMatrix *= glm::scale(scale);

		planetShader->setMat4("modelViewProjectionMatrix", camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix);
		planetShader->setMat4("normalMatrix", glm::transpose(glm::inverse(modelMatrix)));

		if (planetObject.baseData.get()->baseVolume->isOnFrustrum(*camera->frustrum, modelMatrix, scale))
		{
			planetObject.drawObject();
			//std::cout << "Is visible " << '\n';
		}
		/*else {
			std::cout << "NOT visible " << '\n';

		}*/
	}

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
		camera->rotate(UP, deltaTime);
	}
	if( key == 'x'){
		camera->rotate(DOWN, deltaTime);
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
	glutTimerFunc(1000.0f / FPS, frameFunc, 0);
	//atexit(onExit);
	//glutPassiveMotionFunc(My_mouse_routine);
	glutMainLoop();

	return 0;
}

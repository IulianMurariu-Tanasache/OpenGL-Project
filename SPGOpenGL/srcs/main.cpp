#include "Planet.h"
#include "Camera.h"
#include "Shader.h"
#include "VAOObject.h"
#include "BoundingSphere.h"
#include "Texture.h" 
#include "Skybox.h"
#include "QuadObject.h"
#include "FrameBuffer.h"
#include "MainFrameBuffer.h"
#include <stack>
#include <time.h>
#include <iostream>
#include "stb_image.h"
#include "DrawString.h"
#include "PlanetsTexts.h"

#define W_WIDTH 1024
#define W_HEIGHT 720

#define PI glm::pi<float>()
#define FPS 60

#define AU 9.0f //astronomical unit
#define ORBIT_SPEED PI / 2000 
#define EARTH_AXIS_ROTATION PI / 80
#define SUN_OFFSET 6.0f
#define EARTH_SCALE 0.5f

/*
	-make vbo and vao more generic ???
	-cratere pe planete - Normal mapping
	-eclipse - umbre ??
	-adaugat inclinatie la planete ?
	-calcul modelviewprojection matrix in shader + normalMatrix tot in shadeR??
	-jumatate de soare in loc de tot soarele - optimizare ???n
*/
void drawString(unsigned int x, unsigned int y);
int mouseClickX=0, mouseClickY=0;
bool clicked = false;
bool paused = false;

std::unique_ptr<MainFrameBuffer> mainFrameBuffer;

std::vector<Planet> planets;
std::unique_ptr<Planet> sun;

std::unique_ptr<Camera> camera;

std::unique_ptr<Shader> planetShader;
std::unique_ptr<Shader> skyboxShader;
std::unique_ptr<Shader> sunShader;
std::unique_ptr<Shader> blurShader;
std::unique_ptr<Shader> sceneShader;

std::unique_ptr<Skybox> skybox;

std::unique_ptr<QuadObject> quad;

std::unique_ptr<FrameBuffer> pingpong[2];

glm::mat4 modelMatrix;
std::stack<glm::mat4> modelStack;

std::string curr_text = "";

int frame_count = 0;
int start_time, final_time;
int deltaTime = 0;	// Time between current frame and last frame
int lastFrameTime = 0; // Time of last frame

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

	//id, std::shared_ptr<FlyweightObjectComponent> component, std::shared_ptr<Texture> texture, std::shared_ptr<VAOObject> vaoObj,
	//float _axisInclineAngle, float _axisRotAngleInc, float _orbitDist, float _orbitAngle, float _orbitAngleInc
	//planets.push_back(Planet(sphereComp, sunTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 200.0f, 0, 0, 0));
	//planets.back().scale = glm::vec3(14.0f * EARTH_SCALE, 14.0f * EARTH_SCALE, 14.0f * EARTH_SCALE);
	sun = std::make_unique<Planet>(1, sphereComp, sunTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 200.0f, 0, 0, 0);
	sun->scale = glm::vec3(14.0f * EARTH_SCALE, 14.0f * EARTH_SCALE, 14.0f * EARTH_SCALE);

	planets.push_back(Planet(2, sphereComp, mercuryTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 40.0f, 0.4f * AU + SUN_OFFSET, 0, 47.0f * ORBIT_SPEED)); //mercury
	planets.back().scale = glm::vec3((1 / 2.0f) * EARTH_SCALE, (1 / 2.0f) * EARTH_SCALE, (1 / 2.0f) * EARTH_SCALE);
	planets.push_back(Planet(3, sphereComp, venusTexture, vaoObj, 177, -1.0f * (EARTH_AXIS_ROTATION / 100.0f), 0.7f * AU + SUN_OFFSET, 0, 35.02f * ORBIT_SPEED)); //venus
	planets.back().scale = glm::vec3(1 * EARTH_SCALE, 1 * EARTH_SCALE, 1 * EARTH_SCALE);
	planets.push_back(Planet(4, sphereComp, earthTexture, vaoObj, 23, EARTH_AXIS_ROTATION, AU + SUN_OFFSET, 0, 29.78f * ORBIT_SPEED)); //earth
	planets.back().scale = glm::vec3(1 * EARTH_SCALE, 1 * EARTH_SCALE, 1 * EARTH_SCALE);
	planets.push_back(Planet(5 ,sphereComp, marsTexture, vaoObj, 25, EARTH_AXIS_ROTATION, 1.5f * AU + SUN_OFFSET, 0, 26.5f * ORBIT_SPEED)); //mars
	planets.back().scale = glm::vec3((1 / 1.3f) * EARTH_SCALE, (1 / 1.3f) * EARTH_SCALE, (1 / 1.3f) * EARTH_SCALE);
	planets.push_back(Planet(6, sphereComp, jupiterTexture, vaoObj, 3, EARTH_AXIS_ROTATION * 2.4f, 5.0f * AU + SUN_OFFSET, 0, 13.06f * ORBIT_SPEED)); //jupiter
	planets.back().scale = glm::vec3(4.2f * EARTH_SCALE, 4.2f * EARTH_SCALE, 4.2f * EARTH_SCALE);
	planets.push_back(Planet(7, sphereComp, saturnTexture, vaoObj, 27, EARTH_AXIS_ROTATION * 2.2f, 7.5f * AU + SUN_OFFSET, 0, 10 * ORBIT_SPEED)); //saturn
	planets.back().scale = glm::vec3(3.35f * EARTH_SCALE, 3.35f * EARTH_SCALE, 3.35f * EARTH_SCALE);
	planets.push_back(Planet(8, sphereComp, uranusTexture, vaoObj, 98, EARTH_AXIS_ROTATION * 1.4f, 9.8f * AU + SUN_OFFSET, 0, 7.11f * ORBIT_SPEED)); //uranus
	planets.back().scale = glm::vec3(1.6f * EARTH_SCALE, 1.6f * EARTH_SCALE, 1.6f * EARTH_SCALE);
	planets.push_back(Planet(9, sphereComp, neptuneTexture, vaoObj, 28, EARTH_AXIS_ROTATION * 1.5f, 11.1f * AU + SUN_OFFSET, 0, 5.43f * ORBIT_SPEED)); //neptune
	planets.back().scale = glm::vec3(1.61f * EARTH_SCALE, 1.61f * EARTH_SCALE, 1.61f * EARTH_SCALE);
}

void init()
{
	initTexts();
	glewInit();

	camera = std::make_unique<Camera>(W_WIDTH, W_HEIGHT, glm::vec3(0, 60, 70), 40.0f);

	planetShader = std::make_unique<Shader>("shaders/planetShader.vert", "shaders/planetShader.frag");
	planetShader->use();
	planetShader->setVec3("sunPos", glm::vec3(0, 0, 0));
	planetShader->setFloat("sunRadius", 5);
	planetShader->setInt("currentTexture", 0);

	skyboxShader = std::make_unique<Shader>("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);

	sunShader = std::make_unique<Shader>("shaders/sunShader.vert", "shaders/sunShader.frag");
	sunShader->use();
	sunShader->setInt("currentTexture", 0);
	//sunShader->setInt("id", 1);

	blurShader = std::make_unique<Shader>("shaders/gaussianBlur.vert", "shaders/gaussianBlur.frag");

	sceneShader = std::make_unique<Shader>("shaders/sceneShader.vert", "shaders/sceneShader.frag");
	sceneShader->use();
	sceneShader->setInt("scene", 0);
	sceneShader->setInt("bloomBlur", 1);
	sceneShader->setFloat("exposure", 1.5f);

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

	quad = std::make_unique<QuadObject>();

	pingpong[0] = std::make_unique<FrameBuffer>();
	pingpong[0]->init(W_WIDTH, W_HEIGHT);
	pingpong[1] = std::make_unique<FrameBuffer>();
	pingpong[1]->init(W_WIDTH, W_HEIGHT);

	mainFrameBuffer = std::make_unique<MainFrameBuffer>();
	mainFrameBuffer->init(W_WIDTH, W_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::cout << "Avem erori? " << glGetError() << '\n';
}

void display()
{
	mainFrameBuffer->enable();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//draw skybox
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	skyboxShader->use();
	skyboxShader->setMat4("projection", camera->getProjectionMatrix());
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::mat4(glm::mat3(glm::lookAt(camera->cameraPos, camera->cameraPos + camera->cameraFront, camera->cameraUp)));
	skyboxShader->setMat4("view", view);

	skybox->draw();

	//draw rest of the scene
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	unsigned int clear[4] = { 0,0,0,0 };
	glClearTexImage(mainFrameBuffer->colorBuffers[2], 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, clear);
	glm::vec3 scale = { 1,1,1 };
	modelMatrix = glm::mat4();

	//draw sun
	sunShader->use();

	if (!paused) {
		sun->move();
	}
	modelMatrix *= glm::translate(glm::vec3(0, 1, 0));
	modelMatrix *= sun->rotateAroundOrbit();
	modelMatrix *= sun->moveOnOrbit();
	modelMatrix *= sun->inclineAxis();
	modelMatrix *= sun->rotateAroundAxis();
	scale = sun->scale;
	modelMatrix *= glm::scale(scale);

	sunShader->setMat4("modelViewProjectionMatrix", camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix);

	if (sun->getBaseData().baseVolume->isOnFrustrum(*camera->frustrum, modelMatrix, scale))
	{
		sun->drawObject();
	}

	mainFrameBuffer->enable();

	//draw planets
	planetShader->use();
	planetShader->setVec3("viewPos", camera->cameraPos);

	for (Planet& planetObject : planets) {
		if (!paused) {
			planetObject.move();
		}
		modelMatrix = glm::mat4();
		modelMatrix *= glm::translate(glm::vec3(0, 1, 0));
		modelMatrix *= planetObject.rotateAroundOrbit();
		modelMatrix *= planetObject.moveOnOrbit();
		modelMatrix *= planetObject.inclineAxis();
		modelMatrix *= planetObject.rotateAroundAxis();
		scale = planetObject.scale;
		modelMatrix *= glm::scale(scale);

		planetShader->setMat4("modelViewProjectionMatrix", camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix);
		planetShader->setMat4("modelM", modelMatrix);
		planetShader->setMat4("normalMatrix", glm::transpose(glm::inverse(modelMatrix)));
		planetShader->setInt("id", planetObject.id);

		if (planetObject.getBaseData().baseVolume->isOnFrustrum(*camera->frustrum, modelMatrix, scale))
		{
			planetObject.drawObject();
		}
	}
	
	//bloom->nu face nimic pe nvidia cred...
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	blurShader->use();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpong[horizontal]->fbo);
		blurShader->setInt("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? mainFrameBuffer->colorBuffers[1] : pingpong[!horizontal]->colorBuffer);  // bind texture of other framebuffer (or scene if first iteration)
		quad->draw();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	if (clicked)
	{
		mainFrameBuffer->enable();
		glReadBuffer(GL_COLOR_ATTACHMENT2);

		unsigned int pixel[4] = {0,0,0,0};
		glReadPixels(mouseClickX, W_HEIGHT - mouseClickY - 1, 1, 1, GL_RGBA_INTEGER, GL_UNSIGNED_INT, &pixel);

		glReadBuffer(GL_NONE);
		std::cout << "Clicked!->" << pixel[3] << " at " << pixel[1] << " " << pixel[2] << '\n';

		if (pixel[3] > 0 && pixel[3] < 100)
		{
			if (mapTextsById.count(pixel[3]))
				curr_text = mapTextsById[pixel[3]];
			else
				curr_text = "click pe planeta " + std::to_string(pixel[3]);
			paused = true;
		}
		else
		{
			curr_text = "";
		}
	}

	//draw frameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneShader->use();
	modelMatrix = glm::mat4();

	//fa shader separat pentru textbox
	sceneShader->setMat4("mvp", modelMatrix);
	sceneShader->setInt("da", false);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainFrameBuffer->colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpong[!horizontal]->colorBuffer);

	quad->draw();
	
	if (curr_text != "")
	{
		drawString(mouseClickX, W_HEIGHT - mouseClickY);
	}
	clicked = false;

	glutSwapBuffers();
}

void drawString(unsigned int x, unsigned int y)
{
	sceneShader->use();
	QuadObject smallQuad = QuadObject();

	int lines = 1;
	for (int i = 0; i < curr_text.size(); ++i)
	{
		if (curr_text[i] == '\n')
			lines++;
	}

	int string_w = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)curr_text.c_str()) + 10;
	int string_h = glutBitmapHeight(GLUT_BITMAP_HELVETICA_18) * lines + 10;

	float w = string_w * 1.0f;
	float h = string_h * 1.0f;

	w = w / W_WIDTH;
	h = h / W_HEIGHT;

	if (x + string_w > W_WIDTH)
	{
		x = W_WIDTH - string_w;
	}
	if ((int)(y - string_h) < 0)
	{
		y = string_h;
	}

	float m_x = (x) * 2.0f / (W_WIDTH * 1.0f) - 1.0f;
	float m_y = (y) * 2.0f / (W_HEIGHT * 1.0f) - 1.0f;

	modelMatrix = glm::mat4();
	modelMatrix *= glm::translate(glm::vec3(m_x + w, m_y - h, -0.5f));
	modelMatrix *= glm::scale(glm::vec3(w, h, 1.0f));
	sceneShader->setMat4("mvp", modelMatrix);
	sceneShader->setInt("da", true);
	smallQuad.draw();

	glUseProgram(0);
	glWindowPos2i(x , y - 20);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)curr_text.c_str());
	glEnable(GL_LIGHTING);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
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

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q') {
		camera->move(UP, deltaTime);
	}
	if (key == 'e') {
		camera->move(DOWN, deltaTime);
	}
	if (key == 'a') {
		camera->move(LEFT, deltaTime);
	}
	if (key == 'd') {
		camera->move(RIGHT, deltaTime);
	}
	if (key == 'w') {
		camera->move(FORWARDS, deltaTime);
	}
	if (key == 's') {
		camera->move(BACKWARDS, deltaTime);
	}
	if (key == 'p') {
		paused = !paused;
		curr_text = "";
	}
}

bool firstMouse = true;
void mouseCallback(int x, int y)
{
	static int lastX, lastY;
	if (firstMouse)
	{
		glutWarpPointer(W_WIDTH / 2.0f, W_HEIGHT / 2.0f);
		lastX = x = W_WIDTH / 2.0f;
		lastY = y = W_HEIGHT / 2.0f;

		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y;
	lastX = x;
	lastY = y;

	if (!paused) {
		if (xoffset < 0)
		{
			camera->rotate(LEFT, (int)abs(xoffset));
		}
		else
		{
			camera->rotate(RIGHT, (int)abs(xoffset));
		}
		if (yoffset < 0)
		{
			camera->rotate(DOWN, (int)abs(yoffset));
		}
		else
		{
			camera->rotate(UP, (int)abs(yoffset));
		}
	}
}

void mouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseClickX = x;
		mouseClickY = y;
		clicked = true;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 200);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutCreateWindow("Cel mai engine grafic");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	start_time = time(NULL);
	glutTimerFunc(1000.0f / FPS, frameFunc, 0);
	glutPassiveMotionFunc(mouseCallback);
	glutMotionFunc(mouseCallback);
	glutMouseFunc(mouseClick);
	glutMainLoop();

	return 0;
}
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
#include "InputManager.h"
#include <stack>
#include <time.h>
#include <iostream>
#include "stb_image.h"
#include "PlanetsTexts.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define W_WIDTH 1064
#define W_HEIGHT 800

#define PI glm::pi<float>()
#define FPS 60

#define AU 10.5f //astronomical unit
#define ORBIT_SPEED PI / 3000 
#define EARTH_AXIS_ROTATION PI / 80
#define SUN_OFFSET 9.5f
#define EARTH_SCALE 1.2f

unsigned int mouseClickX=0, mouseClickY=0;
bool clicked = false;
bool paused = false;
bool firstMouse = true;

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

InputManager inputManager;

int frame_count = 0;
int start_time, final_time;
int deltaTime = 0;	// Time between current frame and last frame
int lastFrameTime = 0; // Time of last frame

int id_selected = 0;

float scale_multiplier = 1.0f;
float speed_multiplier = 1.0f;
float dist_multiplier = 1.0f;
bool centeredPlanet = false;

void allocPlanets()
{
	//load obj
	std::shared_ptr<FlyweightObjectComponent> sphereComp = std::make_shared<FlyweightObjectComponent>();
	sphereComp->loadOBJFile("obj/sfera-fina.obj");

	//load textures
	std::shared_ptr<Texture> sunTexture = std::make_shared<Texture>("textures/sun.jpg");
	std::shared_ptr<Texture> mercuryTexture = std::make_shared<Texture>("textures/mercury.jpg");
	std::shared_ptr<Texture> venusTexture = std::make_shared<Texture>("textures/venus.jpg");
	std::shared_ptr<Texture> earthTexture = std::make_shared<Texture>("textures/earth.jpg");
	std::shared_ptr<Texture> marsTexture = std::make_shared<Texture>("textures/mars.jpg");
	std::shared_ptr<Texture> jupiterTexture = std::make_shared<Texture>("textures/jupiter.jpg");
	std::shared_ptr<Texture> saturnTexture = std::make_shared<Texture>("textures/saturn.jpg");
	std::shared_ptr<Texture> uranusTexture = std::make_shared<Texture>("textures/uranus.jpg");
	std::shared_ptr<Texture> neptuneTexture = std::make_shared<Texture>("textures/neptune.jpg");

	//sateliti
	std::shared_ptr<Texture> moonTexture = std::make_shared<Texture>("textures/planet_moon.jpg");
	std::shared_ptr<Texture> phobosTexture = std::make_shared<Texture>("textures/phobos.jpg");
	std::shared_ptr<Texture> deimosTexture = std::make_shared<Texture>("textures/deimos.jpg");
	std::shared_ptr<Texture> ioTexture = std::make_shared<Texture>("textures/io.jpg");
	std::shared_ptr<Texture> callistoTexture = std::make_shared<Texture>("textures/callisto.jpg");
	std::shared_ptr<Texture> europaTexture = std::make_shared<Texture>("textures/europa.jpg");
	std::shared_ptr<Texture> iapetusTexture = std::make_shared<Texture>("textures/iapetus.jpg");
	std::shared_ptr<Texture> titanTexture = std::make_shared<Texture>("textures/titan.jpg");
	std::shared_ptr<Texture> titaniaTexture = std::make_shared<Texture>("textures/titania.jpg");

	//create VAO
	std::shared_ptr<VAOObject> vaoObj = std::make_shared<VAOObject>();

	//id, std::shared_ptr<FlyweightObjectComponent> component, std::shared_ptr<Texture> texture, std::shared_ptr<VAOObject> vaoObj,
	//float _axisInclineAngle, float _axisRotAngleInc, float _orbitDist, float _orbitAngle, float _orbitAngleInc
	//planets.push_back(Planet(sphereComp, sunTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 200.0f, 0, 0, 0));
	//planets.back().scale = glm::vec3(14.0f * EARTH_SCALE, 14.0f * EARTH_SCALE, 14.0f * EARTH_SCALE);
	sun = std::make_unique<Planet>(1, sphereComp, sunTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 200.0f, 0, 0, 0);
	sun->scale = glm::vec3(8.5f * EARTH_SCALE, 8.5f * EARTH_SCALE, 8.5f * EARTH_SCALE);

	planets.push_back(Planet(2, sphereComp, mercuryTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 40.0f, 0.5f * AU + SUN_OFFSET, 0, 47.0f * ORBIT_SPEED)); //mercury
	planets.back().scale = glm::vec3((1 / 2.0f) * EARTH_SCALE, (1 / 2.0f) * EARTH_SCALE, (1 / 2.0f) * EARTH_SCALE);
	planets.push_back(Planet(3, sphereComp, venusTexture, vaoObj, 177, -1.0f * (EARTH_AXIS_ROTATION / 100.0f), 0.8f * AU + SUN_OFFSET, 0, 35.02f * ORBIT_SPEED)); //venus
	planets.back().scale = glm::vec3(1 * EARTH_SCALE, 1 * EARTH_SCALE, 1 * EARTH_SCALE);
	
	//earth
	planets.push_back(Planet(4, sphereComp, earthTexture, vaoObj, 23, EARTH_AXIS_ROTATION, AU + SUN_OFFSET, 0, 29.78f * ORBIT_SPEED)); //earth
	Planet& earth = planets.back();
	earth.scale = glm::vec3(1 * EARTH_SCALE, 1 * EARTH_SCALE, 1 * EARTH_SCALE);

	//moon
	Planet moon = Planet(41, sphereComp, moonTexture, vaoObj, 0, EARTH_AXIS_ROTATION / 15.0f, AU / 2.6f, 0, 12.0f * ORBIT_SPEED);
	moon.scale = glm::vec3((1 / 2.2f) * EARTH_SCALE, (1 / 2.2f) * EARTH_SCALE, (1 / 2.2f) * EARTH_SCALE);
	earth.sattelites.push_back(moon);

	//mars
	planets.push_back(Planet(5 ,sphereComp, marsTexture, vaoObj, 25, EARTH_AXIS_ROTATION, 1.8f * AU + SUN_OFFSET, 0, 26.5f * ORBIT_SPEED)); //mars
	Planet& mars = planets.back();
	mars.scale = glm::vec3((1 / 1.1f) * EARTH_SCALE, (1 / 1.1f) * EARTH_SCALE, (1 / 1.1f) * EARTH_SCALE);

	//phobos
	Planet phobos = Planet(51, sphereComp, phobosTexture, vaoObj, 15, EARTH_AXIS_ROTATION / 15.0f, AU / 2.25f, 60, ORBIT_SPEED * 10.0f);
	phobos.scale = glm::vec3((1 / 3.0f) * EARTH_SCALE, (1 / 3.0f) * EARTH_SCALE, (1 / 3.0f) * EARTH_SCALE);
	mars.sattelites.push_back(phobos);

	//deimos
	Planet deimos = Planet(52, sphereComp, deimosTexture, vaoObj, 19, EARTH_AXIS_ROTATION / 32.0f, AU / 2.0f, 25, ORBIT_SPEED * 30.0f);
	deimos.scale = glm::vec3((1 / 2.75f) * EARTH_SCALE, (1 / 2.75f) * EARTH_SCALE, (1 / 2.75f) * EARTH_SCALE);
	mars.sattelites.push_back(deimos);

	//jupiter
	planets.push_back(Planet(6, sphereComp, jupiterTexture, vaoObj, 3, EARTH_AXIS_ROTATION * 2.0f, 3.5f * AU + SUN_OFFSET, 0, 13.06f * ORBIT_SPEED)); //jupiter
	Planet& jupiter = planets.back();
	jupiter.scale = glm::vec3(3.8f * EARTH_SCALE, 3.8f * EARTH_SCALE, 3.8f * EARTH_SCALE);

	//io
	Planet io = Planet(61, sphereComp, ioTexture, vaoObj, -12, EARTH_AXIS_ROTATION / 42.0f, AU / 1.5f, 15, ORBIT_SPEED * 45.0f);
	io.scale = glm::vec3(EARTH_SCALE / 1.8f);
	jupiter.sattelites.push_back(io);

	//callisto
	Planet callisto = Planet(62, sphereComp, callistoTexture, vaoObj, 2, EARTH_AXIS_ROTATION / 26.0f, AU, 29, ORBIT_SPEED * 11.0f);
	callisto.scale = glm::vec3(EARTH_SCALE / 1.1f);
	jupiter.sattelites.push_back(callisto);

	//europa
	Planet europa = Planet(63, sphereComp, europaTexture, vaoObj, 21, EARTH_AXIS_ROTATION / 30.0f, AU * 1.5f, 55, ORBIT_SPEED * 26.0f);
	europa.scale = glm::vec3(EARTH_SCALE / 1.5f);
	jupiter.sattelites.push_back(europa);

	//saturn
	planets.push_back(Planet(7, sphereComp, saturnTexture, vaoObj, 27, EARTH_AXIS_ROTATION * 2.2f, 5.5f * AU + SUN_OFFSET, 0, 10 * ORBIT_SPEED)); //saturn
	Planet& saturn = planets.back();
	saturn.scale = glm::vec3(3.15f * EARTH_SCALE, 3.15f * EARTH_SCALE, 3.15f * EARTH_SCALE);

	//titan
	Planet titan = Planet(71, sphereComp, titanTexture, vaoObj, 5, EARTH_AXIS_ROTATION / 38.0f, AU, 55, ORBIT_SPEED * 19.0f);
	titan.scale = glm::vec3(EARTH_SCALE * 1.2f);
	saturn.sattelites.push_back(titan);

	//iapetus
	Planet iapetus = Planet(72, sphereComp, europaTexture, vaoObj, 50, EARTH_AXIS_ROTATION / 40.0f, AU * 1.85f, 10, ORBIT_SPEED * 12.0f);
	iapetus.scale = glm::vec3(EARTH_SCALE / 1.35f);
	saturn.sattelites.push_back(iapetus);

	//uranus
	planets.push_back(Planet(8, sphereComp, uranusTexture, vaoObj, 98, EARTH_AXIS_ROTATION * 1.4f, 7.8f * AU + SUN_OFFSET, 0, 7.11f * ORBIT_SPEED)); //uranus
	Planet& uranus = planets.back();
	uranus.scale = glm::vec3(1.5f * EARTH_SCALE, 1.5f * EARTH_SCALE, 1.5f * EARTH_SCALE);

	//titania
	Planet titania = Planet(81, sphereComp, titaniaTexture, vaoObj, 36, EARTH_AXIS_ROTATION / 30.0f, AU, 0, ORBIT_SPEED * 34.0f);
	titania.scale = glm::vec3(EARTH_SCALE / 1.22f);
	uranus.sattelites.push_back(titania);

	//neptune
	planets.push_back(Planet(9, sphereComp, neptuneTexture, vaoObj, 28, EARTH_AXIS_ROTATION * 1.5f, 8.81f * AU + SUN_OFFSET, 0, 5.43f * ORBIT_SPEED)); //neptune
	Planet& neptune = planets.back();
	neptune.scale = glm::vec3(1.61f * EARTH_SCALE, 1.61f * EARTH_SCALE, 1.61f * EARTH_SCALE);
}

void drawPlanet(Planet& planetObject, const glm::mat4& modelM)
{
	glm::vec3 scale = { 1,1,1 };

	glm::mat4 modelMatrix = modelM;
	planetObject.move(speed_multiplier * (!(id_selected == planetObject.id && centeredPlanet == true)));
	modelMatrix *= glm::translate(glm::vec3(0, 1, 0));
	modelMatrix *= planetObject.rotateAroundOrbit();
	modelMatrix *= planetObject.moveOnOrbit(dist_multiplier); //* (scale_multiplier));

	//sateliti
	modelStack.push(modelMatrix);

	modelMatrix *= planetObject.inclineAxis();
	modelMatrix *= planetObject.rotateAroundAxis();
	planetObject.modelMatrix = modelMatrix;
	scale = planetObject.scale;
	modelMatrix *= glm::scale(scale * scale_multiplier);

	planetShader->setMat4("modelViewProjectionMatrix", camera->getProjectionMatrix() * camera->getViewMatrix() * modelMatrix);
	planetShader->setMat4("modelM", modelMatrix);
	planetShader->setMat4("normalMatrix", glm::transpose(glm::inverse(modelMatrix)));
	planetShader->setInt("id", planetObject.id);

	if (planetObject.getBaseData().baseVolume->isOnFrustrum(*camera->frustrum, modelMatrix, scale))
	{
		planetObject.drawObject();
	}
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

static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void my_display_code()
{
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

	ImGui::Begin("UI");

	std::string mouse_cuplat = "Mouse prins de camera!";
	if (paused)
		mouse_cuplat = "Mouse liber!";
	else
		mouse_cuplat = "Mouse prins de camera!";

	ImGui::Text(mouse_cuplat.c_str());
	ImGui::SliderFloat("Size", &scale_multiplier, 0, 10.0f);
	ImGui::SliderFloat("Speed", &speed_multiplier, 0, 2.0f);
	ImGui::SliderFloat("Distance", &dist_multiplier, 0, 5.0f);

	if (ImGui::Button("Reset Camera", ImVec2(0, 0)))
	{
		camera->cameraPos = glm::vec3(0, 60, 70);
		camera->rotate(DOWN, 40.0f);
		camera->rotate(RIGHT, 90.0f);
	}

	if (id_selected > 0)
	{
		ImGui::Text(curr_text.c_str());
		if (ImGui::Button("Go to planet", ImVec2(0, 0)))
		{

			Planet* selected_planet;
			if (id_selected == 1)
				selected_planet = sun.get();
			else {
				if(id_selected < 10)
					selected_planet = &(planets.at(id_selected - 2));
				else {
					selected_planet = &(planets.at((id_selected / 10) - 2).sattelites.at((id_selected % 10) - 1));
				}
			}

			glm::vec3 pos, size;
			selected_planet->getGlobalDimensions(pos, size);
			size *= scale_multiplier;
			pos.z += 9 * size.z;
			paused = true;
			firstMouse = true;
			camera->cameraPos = pos;
			camera->rotate(RIGHT, 90.0f);
			camera->rotate(UP, 0.0f);
			camera->rotate(FORWARDS, 0.0f);
			centeredPlanet = true;
		}
	}

	ImGui::End();
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

	sun->move(speed_multiplier);
	modelMatrix *= glm::translate(glm::vec3(0, 1, 0));
	modelMatrix *= sun->rotateAroundOrbit();
	modelMatrix *= sun->moveOnOrbit();
	modelMatrix *= sun->inclineAxis();
	modelMatrix *= sun->rotateAroundAxis();
	scale = sun->scale;
	modelMatrix *= glm::scale(scale * scale_multiplier);

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
		
		drawPlanet(planetObject, glm::mat4());
		for (Planet& satteliteObject : planetObject.sattelites)
		{
			drawPlanet(satteliteObject, modelStack.top());
			modelStack.pop();
		}
		modelStack.pop();
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
			{
				curr_text = mapTextsById[pixel[3]];
				id_selected = pixel[3];
				paused = true;
				firstMouse = true;
			}
			else
			{
				curr_text = "click pe planeta " + std::to_string(pixel[3]);
				id_selected = 0;
			}
	
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
		//drawString(mouseClickX, W_HEIGHT - mouseClickY);
	}
	clicked = false;
}

void frameFunc() {
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
}

void keyboard()
{
	if (inputManager.getKeyStatus(GLFW_KEY_Q)) {
		camera->move(UP, deltaTime);
	}
	if (inputManager.getKeyStatus(GLFW_KEY_E)) {
		camera->move(DOWN, deltaTime);
	}
	if (inputManager.getKeyStatus(GLFW_KEY_A)) {
		camera->move(LEFT, deltaTime);
	}
	if (inputManager.getKeyStatus(GLFW_KEY_D)) {
		camera->move(RIGHT, deltaTime);
	}
	if (inputManager.getKeyStatus(GLFW_KEY_W)) {
		camera->move(FORWARDS, deltaTime);
	}
	if (inputManager.getKeyStatus(GLFW_KEY_S)) {
		camera->move(BACKWARDS, deltaTime);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (inputManager.getKeyStatus(GLFW_KEY_P)) {
		paused = !paused;
		curr_text = "";
		centeredPlanet = false;
		if (paused)
			firstMouse = true;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		inputManager.getMousePos(mouseClickX, mouseClickY);
		clicked = true;
	}
}

void mouseCallback(int x, int y)
{
	static int lastX, lastY;
	if (firstMouse)
	{
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

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char** argv)
{
	GLFWwindow* window;

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 0;

	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return 0;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(1000, 600);
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	init();

	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		frameFunc();
		glfwPollEvents();

		inputManager.pollEvents(window);
		keyboard();

		unsigned int x, y;
		inputManager.getMousePos(x, y);
		mouseCallback(x, y);

		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		my_display_code();

		////Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		io.WantCaptureMouse = false;
		display();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
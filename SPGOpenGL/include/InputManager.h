#include "glfw3.h"

class InputManager {
private:
	bool keyStatus[27];
	//bool mouseClicks[2];
	double xpos, ypos;

public:
	InputManager();
	void pollEvents(GLFWwindow* window);
	bool getKeyStatus(unsigned int key);
	void getMousePos(unsigned int& mouseX, unsigned int& mouseY);
};
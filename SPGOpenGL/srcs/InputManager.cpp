#include "InputManager.h"

InputManager::InputManager()
{
	for (int i = 0; i < 27; ++i)
	{
		keyStatus[i] = false;
	}
	xpos = 0;
	ypos = 0;
}


void InputManager::pollEvents(GLFWwindow* window)
{
	for (int i = 0; i < 27; ++i)
	{
		int state = glfwGetKey(window, GLFW_KEY_A + i);
		if (state == GLFW_PRESS)
		{
			keyStatus[i] = true;
		}
		else if (state == GLFW_RELEASE)
		{
			keyStatus[i] = false;
		}
	}

	//mouse position
	glfwGetCursorPos(window, &xpos, &ypos);
}

void InputManager::getMousePos(unsigned int& mouseX, unsigned int& mouseY)
{
	mouseX = xpos;
	mouseY = ypos;
}

bool InputManager::getKeyStatus(unsigned int key)
{
	if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
		return keyStatus[key - GLFW_KEY_A];
	}
}
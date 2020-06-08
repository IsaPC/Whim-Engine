#pragma once
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	std::string title;
	unsigned int width;
	unsigned int height;


	GLFWwindow* window = nullptr;

	Window();

	Window(std::string title,
		unsigned int width,
		unsigned int height
	);

	void init();
};


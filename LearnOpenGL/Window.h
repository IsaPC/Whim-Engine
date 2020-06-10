#pragma once
#include <glad/glad.h>
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
	~Window();

	Window(std::string title,
		unsigned int width,
		unsigned int height
	);

private:
	//Shader scripts
	unsigned int VAO;
	unsigned int VBO;

private:
	void init();
	//void run();
	//void buffer();
	// static unsigned int compileShader(unsigned int type, std::string& source);
	// static unsigned int createShader(std::string& vShader, std::string& fShader);
	void buffer();
	void test();

	void processInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}
};


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
	std::string vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	std::string fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

private:
	void init();
	//void run();
	//void buffer();
	static unsigned int compileShader(unsigned int type, std::string& source);
	static unsigned int createShader(std::string& vShader, std::string& fShader);
};


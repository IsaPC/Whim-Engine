#pragma once
#include <iostream>
// reading files
#include <fstream>
#include <sstream>

// glfw
#include <glad/glad.h>


class Shader
{
private:
	unsigned int rendererId;
	const char* vFilePath;
	const char* fFilePath;
public:
	Shader(const std::string vertexShaderSrc, std::string fragmentShaderSrc);
	~Shader();

	void Bind() const;
	void UnBind() const;

private:
	// getting creating and compiling the shader
	std::string ParseShaderScript(std::string fileLocation);
	int CreateShader(unsigned int type, std::string shaderScript);
	int CompileShader(std::string vertexShader, std::string fragmentShader);

public:
	// uniform management
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	int GetUniformLocation(const std::string name);
};



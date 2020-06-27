#include "Shader.h"

Shader::Shader(const std::string vertexShaderSrc, std::string fragmentShaderSrc)
	: rendererId(0),
	vFilePath(vertexShaderSrc.c_str()),
	fFilePath(vertexShaderSrc.c_str())
{
	std::string vSource = ParseShaderScript(vertexShaderSrc);
	std::string fSource = ParseShaderScript(fragmentShaderSrc);
	rendererId = CompileShader(vSource, fSource);

}

Shader::~Shader()
{
}

void Shader::Bind() const
{
	glUseProgram(rendererId);
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

std::string Shader::ParseShaderScript(std::string fileLocation)
{
	std::ifstream shaderFile;
	std::string shaderCode;
	try {
		shaderFile.open(fileLocation);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "Error, could not open and read file" << std::endl;
	}

	return shaderCode;
}

int Shader::CreateShader(unsigned int type, std::string shaderScript)
{
    // type options GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
// create shader program on GPU
    int shader = glCreateShader(type);

    // format string
    const char* source = shaderScript.c_str();

    // assign script to program
    glShaderSource(shader, 1, &source, NULL);

    // compile the shader script
    glCompileShader(shader);

    //check for errors if the compilation failed
    std::string shaderType;
    switch (type)
    {
    case GL_VERTEX_SHADER:
        shaderType = "Vertex";
        break;
    case GL_FRAGMENT_SHADER:
        shaderType = "Fragment";
        break;
    default:
        shaderType = "NO";
        break;
    }

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR: failed to create " << shaderType << " shader\n" << infoLog << std::endl;
    }

    // return the id of the compiled shader
    return shader;
}

int Shader::CompileShader(std::string vertexShader, std::string fragmentShader)
{
    int vs = CreateShader(GL_VERTEX_SHADER, vertexShader);
    int fs = CreateShader(GL_FRAGMENT_SHADER, fragmentShader);

    int program = glCreateProgram();
    glAttachShader(program, fs);
    glAttachShader(program, vs);
    glLinkProgram(program);
    glValidateProgram(program);
    // check for linking errors

   //check for errors of the program
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

int Shader::GetUniformLocation(const std::string name)
{
    int location = glGetUniformLocation(rendererId, name.c_str());
    return location;
}

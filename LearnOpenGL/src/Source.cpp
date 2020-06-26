// Author: Isaac Ceff
/* learning Refereces:
https://learnopengl.com/
https://www.udemy.com/course/graphics-with-modern-opengl/
https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/

#include <iostream>
#include <fstream>
#include <sstream>

// opengl libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "ElementArrayBuffer.h"

//window handling
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// shader
std::string parseShaderScript(std::string fileLocation);
int createShader(unsigned int type, std::string shaderScript);
int compileShader(unsigned int vertexShader, unsigned int fragmentShader);

//test, does texture load
int loadTexture(std::string imageLocation) {
    return 0;
}

// windows setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* title = "main Window";
GLFWwindow* window;


std::string parseShaderScript(std::string fileLocation)
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

int createShader(unsigned int type, std::string shaderScript) {
    // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
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

int compileShader(std::string vertexShader, std::string fragmentShader) {
    int vs = createShader(GL_VERTEX_SHADER, vertexShader);
    int fs = createShader(GL_FRAGMENT_SHADER, fragmentShader);

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

void init(int src_width, int src_heright, std::string title) {
    // initialise GLFW
    if (!glfwInit()) {
        std::cout << "glfw failed to initilise" << std::endl;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // create window
    const char* windowTitle = title.c_str();
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowTitle, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}



int main()
{
    // initilse requirments
    init(SCR_WIDTH, SCR_HEIGHT, title);

    //create vetex shader
    std::string vShaderSource = parseShaderScript("shader_scripts/vs.shader");
    std::string fShaderSource = parseShaderScript("shader_scripts/fs.shader");
    unsigned int shaderProgram = compileShader(vShaderSource, fShaderSource);


    {
        float verts[] = {
           -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f,  0.5f,
           -0.5f, 0.5f
        };

        //these indices tells the order to that generates the shapes (a sqaure)
        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        // create a vertix buffer object on gpu
        VertexArray va;
        VertexBuffer vb(verts, 4 * 2 * sizeof(float));

        // create the layout of the data
        VertexBufferLayout layout;
        layout.Push<float>(2);

        // add the buffer to gpu that follows the layout
        va.addBuffer(vb, layout);


        ElementArrayBuffer ib(indicies, 6);

        glUseProgram(shaderProgram);

        //send data to the bound shader (assign color) 
        int location = glGetUniformLocation(shaderProgram, "u_Color");
        glUniform4f(location, 4.0f, 0.0f, 0.0f, 1.0f);

        //unbind buffers
        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // input
            // -----
            processInput(window);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw our first triangle
            glUseProgram(shaderProgram);


            va.Bind();
            ib.Bind();

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    
    // delete shader program
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
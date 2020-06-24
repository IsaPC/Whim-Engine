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

// for loading textures
#define STB_IMAGE_IMPLEMENTATION
#include "STB_IMAGE/stb_image.h"

std::string parseShaderScript(std::string fileLocation);

//window handling
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// shader
int createShader(unsigned int type, std::string shaderScript);
int configProgram(unsigned int vertexShader, unsigned int fragmentShader);

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

int configProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // check for linking errors

   //check for errors of the program
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
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
    int shaderProgram = compileShader(vShaderSource, fShaderSource);

    //int vertexShader = createShader(GL_VERTEX_SHADER, vShaderSource);
    // create fragment shader
   // int fragmentShader = createShader(GL_FRAGMENT_SHADER, fShaderSource);
    // create te shader program from the shaders
    //int program = configProgram(vertexShader, fragmentShader);

    // configure texture



    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

    };


    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


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


        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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
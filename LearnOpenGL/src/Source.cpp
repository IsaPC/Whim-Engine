// Author: Isaac Ceff
/* learning Refereces:
https://learnopengl.com/
https://www.udemy.com/course/graphics-with-modern-opengl/
https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/

#include <iostream>

// opengl libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>


//buffers
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "ElementArrayBuffer.h"

//Shader
#include "Shader.h"


//window handling
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


//test, does texture load
int loadTexture(std::string imageLocation) {
    return 0;
}

// windows setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* title = "main Window";
GLFWwindow* window;


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

        /// Shader ///
        Shader shader("shader_scripts/vs.shader", "shader_scripts/fs.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 4.0f, 0.0f, 0.0f, 1.0f);

        //unbind buffers
        va.UnBind();
        vb.UnBind();
        ib.UnBind();
        shader.UnBind();


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

            //change the color of the square
            shader.Bind();
            shader.SetUniform4f("u_Color", 4.0f, 0.0f, 0.0f, 1.0f);



            va.Bind();
            ib.Bind();

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

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
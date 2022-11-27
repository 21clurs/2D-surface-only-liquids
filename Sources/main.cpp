#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"math.h"

#include"display.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"texture.h"

int main()
{
    Display display(800,600,"Hello World");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    // Tell GLFW we are using the CORE profile -- only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(800, 800, "Bonjour!", NULL, NULL);
    // Error check if the window fails to exist :')
    if (window == NULL){
        std::cout<<"Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Make window part of current "context"-- object holding the whole of opengl
    glfwMakeContextCurrent(window); 
    // Load GLAD so it configures OpenGL
    gladLoadGL();
    // Specifies the viewport of OpenGL in the window-- here it goes from (0,0) to (800,800)
    glViewport(0,0,800, 800);

    // Vertex coordinates
    GLfloat vertices[] =
    {
        /*-0.5f, -0.5f*float(sqrt(3))/3,  0.0f,   0.8f, 0.3f, 0.02f,
        0.5f, -0.5f*float(sqrt(3))/3,   0.0f,   0.8f, 0.3f, 0.02f,
        0.0f, 0.5f*float(sqrt(3))*2/3,  0.0f,   1.0f, 0.6f, 0.32f,

        -0.5f/2, 0.5f*float(sqrt(3))/6, 0.0f,   0.9f, 0.45f, 0.17f, 
        0.5f/2, 0.5f*float(sqrt(3))/6,  0.0f,   0.9f, 0.45f, 0.17f,
        0.0f, -0.5f*float(sqrt(3))*2/6, 0.0f,   0.8f, 0.3f, 0.02f,*/

        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,      0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    };

    GLuint indices[] = 
    {
        /*0,3,5,
        3,2,4,
        5,4,1,*/
        
        0,1,2,
        0,2,3,
    };

    Shader shaderProgram("default.vert","default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1,0,3, GL_FLOAT,8*sizeof(float),(void*)0); // Link attributes for position
    VAO1.LinkAttrib(VBO1,1,3, GL_FLOAT,8*sizeof(float),(void*)(3*sizeof(float))); // Link attributes for color
    VAO1.LinkAttrib(VBO1,2,2, GL_FLOAT,8*sizeof(float),(void*)(6*sizeof(float))); // Link attributes for texture
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Gets ID of uniform called "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    popCat.texUnit(shaderProgram, "tex0", 0);

    // Set color of back buffer
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // Color of background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();
        // Can only call this after Activating Shader Program. Assigns value to uniform
        glUniform1f(uniID,0.5f);
        popCat.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw the triangle using GL_TRIANGLES primitive
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    popCat.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


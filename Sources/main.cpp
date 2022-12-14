#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<tuple>
#include<vector>

#include"math.h"

#include"display.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"texture.h"
#include"vertex.h"

void advectFE(float timestep, Vertex* vertices, int nVertices);
void advectRK2(float timestep, Vertex* vertices, int nVertices);
std::tuple<float, float> evalVecField(float xPos, float yPos);

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

    // Vertices
    int numVertex = 3;
    Vertex vertexArr[] = {
        Vertex(0.1f, 0.1f), //Vertex(-0.5f, -0.5f*float(sqrt(3))/3),
        Vertex(0.1f, 0.2f), //Vertex(0.5f, -0.5f*float(sqrt(3))/3),
        Vertex(0.2f, 0.1f), //Vertex(0.0f, 0.5f*float(sqrt(3))*2/3),
    };
    // Velocities
    // Just initializing all velocities to 0
    std::vector<std::tuple<float,float>> velocities(numVertex, std::make_tuple(0,0));
    // Constructing 'edges' vector
    // Contains tuples (i,j) where there exists an edge between vertexArr[i] and vertexArr[j] 
    int numEdges = numVertex;
    std::vector<std::tuple<int,int>> edges;
    for(int i=0; i<numEdges; i++){
        edges.push_back(std::make_tuple(i,(i+1)%numVertex)); // Super simple for this case
    }
    // Constructing 'vertToEdges' vector
    // vertToEdges[i] is a tuple (j,k) where vertexArr[i] is connected to edges[i] and edges[j] 
    std::vector<std::tuple<int, int>> vertToEdges(numVertex, std::make_tuple(0,0));
    for(int i=0; i<numEdges; i++){ // Go through edges array to construct --> THIS IS PRETTY SENSITIVE RIGHT NOW :') 
        std::get<0>(vertToEdges[std::get<0>(edges[i])]) = i;
        std::get<1>(vertToEdges[std::get<1>(edges[i])]) = i;
    }

    // Just printing to check on things properly setup
    /*for(int i=0; i<numEdges; i++){
        std::cout<<"("<<std::get<0>(edges[i])<<","<<std::get<1>(edges[i])<<")";
    }
    std::cout<<std::endl;
    for(int i=0; i<numVertex; i++){
        std::cout<<"("<<std::get<0>(vertToEdges[i])<<","<<std::get<1>(vertToEdges[i])<<")";
    }
    std::cout<<std::endl;*/

    GLfloat vertices[numVertex*3];
    verticesToArr(vertexArr, numVertex, vertices);

    // Vertex coordinates
    //GLfloat vertices[] =
    //{
    //    -0.5f, -0.5f*float(sqrt(3))/3,  0.0f,   //0.8f, 0.3f, 0.02f,
    //    0.5f, -0.5f*float(sqrt(3))/3,   0.0f,   //0.8f, 0.3f, 0.02f,
    //    0.0f, 0.5f*float(sqrt(3))*2/3,  0.0f,   //1.0f, 0.6f, 0.32f,

        /*-0.5f/2, 0.5f*float(sqrt(3))/6, 0.0f,   0.9f, 0.45f, 0.17f, 
        0.5f/2, 0.5f*float(sqrt(3))/6,  0.0f,   0.9f, 0.45f, 0.17f,
        0.0f, -0.5f*float(sqrt(3))*2/6, 0.0f,   0.8f, 0.3f, 0.02f,*/

    //    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    //    -0.5f, 0.5f, 0.0f,      0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    //    0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    //    0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    //};

    GLuint indices[] = 
    {
        0,1,2,

        // TRIANGLES
        /*0,3,5,
        3,2,4,
        5,4,1,*/
        
        // SQUARE
        /*0,1,2,
        0,2,3,*/
    };

    Shader shaderProgram("default.vert","default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1,0,3, GL_FLOAT,3*sizeof(float),(void*)0); // Link attributes for position
    //VAO1.LinkAttrib(VBO1,0,3, GL_FLOAT,8*sizeof(float),(void*)0); // Link attributes for position
    //VAO1.LinkAttrib(VBO1,1,3, GL_FLOAT,8*sizeof(float),(void*)(3*sizeof(float))); // Link attributes for color
    //VAO1.LinkAttrib(VBO1,2,2, GL_FLOAT,8*sizeof(float),(void*)(6*sizeof(float))); // Link attributes for texture
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Gets ID of uniform called "scale"
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
/*
    // Texture
    Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    popCat.texUnit(shaderProgram, "tex0", 0);
*/
    // Set color of back buffer
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    double prevTime = glfwGetTime();

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // Color of background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();

        // Simple timer
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1/60)
        {
            advectRK2(crntTime - prevTime, vertexArr, numVertex);
            prevTime = crntTime;
        }
        // Put vertex coordinate data into vertices
        verticesToArr(vertexArr, numVertex, vertices);

        // Can only call this after Activating Shader Program. Assigns value to uniform
        glUniform1f(uniID,0.5f);
        //popCat.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();

        // A REALLY CLUNKY way to set up all the vertices again and make them be used to draw...
        VBO1.LoadBufferData(vertices, sizeof(vertices)); // Clunky...
        VAO1.LinkAttrib(VBO1,0,3, GL_FLOAT,3*sizeof(float),(void*)0); // Link attributes for position

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
    //popCat.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void advectFE(float timestep, Vertex* vertices, int nVertices){
    for(int i=0; i<=nVertices; i++){
        auto [f_x, f_y] = evalVecField(vertices[i].xPos, vertices[i].yPos); // updates velocities on each vertex
        vertices[i].xPos += f_x*timestep;
        vertices[i].yPos += f_y*timestep;
    }
}

// RK2 explicit midpoint method
void advectRK2(float timestep, Vertex* vertices, int nVertices){
    for(int i=0; i<=nVertices; i++){
        auto [k1_x, k1_y] = evalVecField(vertices[i].xPos, vertices[i].yPos);
        k1_x*=timestep;
        k1_y*=timestep;

        auto [k2_x, k2_y] = evalVecField(vertices[i].xPos + k1_x/2, vertices[i].yPos + k1_x/2);
        k2_x*=timestep;
        k2_y*=timestep;

        vertices[i].xPos += k2_x;
        vertices[i].yPos += k2_y;
    }
}

// evaluate vector field -- 'velocities' -- at a given position
std::tuple<float, float> evalVecField(float xPos, float yPos){
    float xVel = -1*yPos;
    float yVel = xPos;
    return  std::make_tuple(xVel, yVel);
}

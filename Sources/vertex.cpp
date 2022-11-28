#include"vertex.h"

Vertex::Vertex(GLfloat x, GLfloat y): xPos(x), yPos(y), zPos(0.0f), xVel(0.0f), yVel(0.0f), zVel(0.0f){}

void verticesToArr(Vertex* vertexArr, int n, GLfloat* vertices)
{
    for (int i=0; i<n; i++){
        vertices[i*3] = vertexArr[i].xPos;
        vertices[i*3+1] = vertexArr[i].yPos;
        vertices[i*3+2] = vertexArr[i].zPos;
    }
}

void Vertex::advect(float timestep){
    xPos += xVel*timestep;
    yPos += yVel*timestep;
    zPos += zVel*timestep;
}
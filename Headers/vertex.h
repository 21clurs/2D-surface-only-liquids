#ifndef VERTEX_CLASS_H
#define VERTEX_CLASS_H

#include<glad/glad.h>

class Vertex
{
    public:
        GLfloat xPos;
        GLfloat yPos;
        GLfloat zPos;

        float xVel;
        float yVel;
        float zVel;

        Vertex(GLfloat x, GLfloat y);
};

void verticesToArr(Vertex* vertices, int n, GLfloat* verts);

#endif
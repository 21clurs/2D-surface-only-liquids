#ifndef VERTEX_CLASS_H
#define VERTEX_CLASS_H

#include<glad/glad.h>

class Vertex
{
    public:
        GLfloat xPos;
        GLfloat yPos;

        Vertex(GLfloat x, GLfloat y);
};

#endif
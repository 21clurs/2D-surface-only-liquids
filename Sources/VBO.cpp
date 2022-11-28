#include "VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &ID);
    // Bind the VBO, specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW); // DRAW: modified and used to draw image
    
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::Delete()
{
    glDeleteBuffers(1, &ID);
}

void VBO::LoadBufferData(GLfloat* vertices, GLsizeiptr size)
{
    glGenBuffers(1, &ID);
    // Bind the VBO, specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW); // DRAW: modified and used to draw image
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
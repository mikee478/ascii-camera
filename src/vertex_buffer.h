#pragma once

#include <GL/glew.h>

class VertexBuffer
{
public:
    VertexBuffer(const void *data, unsigned int size, unsigned int usage = GL_STATIC_DRAW);
    ~VertexBuffer();
    void UpdateBuffer(const void *data, unsigned int size);
    void Bind() const;
    void Unbind() const;
private:
    unsigned int id_;    
};
#include "index_buffer.h"
#include "renderer.h"

#include <GL/glew.h>

IndexBuffer::IndexBuffer(const void *data, unsigned int count):
    count_(count)
{
    // Create buffer
    GLCALL(glGenBuffers(1, &id_));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_));

    // Put data into buffers (target, size in bytes, *data, usage)
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &id_));
}

void IndexBuffer::Bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_));
}

void IndexBuffer::Unbind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

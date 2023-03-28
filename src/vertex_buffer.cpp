#include "vertex_buffer.h"
#include "renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size, unsigned int usage)
{
    // Create buffer
    GLCALL(glGenBuffers(1, &id_));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, id_));

    // Put data into buffers (target, size in bytes, *data, usage)
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &id_));
}

void VertexBuffer::UpdateBuffer(const void *data, unsigned int size)
{
    Bind();
    GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data))
}

void VertexBuffer::Bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, id_));
}

void VertexBuffer::Unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
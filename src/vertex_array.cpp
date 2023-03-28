#include "vertex_array.h"
#include "renderer.h"

#include <GL/glew.h>

VertexArray::VertexArray()
{
    GLCALL(glGenVertexArrays(1, &id_));
}

VertexArray::~VertexArray()
{
    GLCALL(glDeleteBuffers(1, &id_));
}

void VertexArray::Bind() const
{
    GLCALL(glBindVertexArray(id_));
}

void VertexArray::Unbind() const
{
    GLCALL(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();

    const auto &elements = layout.GetElements();
    unsigned int offset = 0;

    for(int i = 0; i < elements.size(); i++)
    {
        const auto &e = elements[i];
        /* glVertexAttribPointer
            GLuint index - index of the generic vertex attribute
            GLint size - number of components per generic vertex attribute (1,2,3,4)
            GLenum type - data type of each component in the array
            GLboolean normalized - specifies whether fixed-point data values should be normalized
            GLsizei stride - specifies the byte offset between consecutive generic vertex attributes
            const void * pointer - specifies a offset of the first component of the first generic vertex attribute
        
            stride = 0 means data is tightly packed (xyzxyzrgbrgb) xyz is consecutive, rgb is consecutive)
        */
        GLCALL(glVertexAttribPointer(i, e.count, e.type, e.normalized, layout.GetStride(), reinterpret_cast<const void*>(offset)));
        GLCALL(glEnableVertexAttribArray(i));
        offset += e.count * VertexBufferElement::GetSizeOfType(e.type);
    }
}


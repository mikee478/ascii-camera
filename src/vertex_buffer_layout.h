#pragma once

#include <GL/glew.h>
#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            default:
                assert(false);
                return 0;
        }
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout() : stride_(0) {};

    template<typename T>
    void Push(unsigned int)
    {
        static_assert(sizeof(T) == 0);
    }

    template<>
    void Push<float>(unsigned int count)
    {
        elements_.push_back({GL_FLOAT, count, GL_FALSE});
        stride_ += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        elements_.push_back({GL_UNSIGNED_INT, count, GL_TRUE});
        stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    const std::vector<VertexBufferElement>& GetElements() const {return elements_;}
    unsigned int GetStride() const {return stride_;}

private:
    std::vector<VertexBufferElement> elements_;
    unsigned int stride_;
};
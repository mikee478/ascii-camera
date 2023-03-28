#pragma once

class IndexBuffer
{
public:
    IndexBuffer(const void *data, unsigned int count);
    ~IndexBuffer();
    void Bind() const;
    void Unbind() const;
    unsigned int GetCount() const {return count_;}
private:
    unsigned int id_;  
    unsigned int count_;  
};
#include "renderer.h"
#include <GL/glew.h>

void GLClearErrors()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error - (" << error << ") - " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Clear()
{
    // Clears color buffer
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
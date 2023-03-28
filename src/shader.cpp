#include "shader.h"
#include "renderer.h"

#include <fstream>
#include <sstream>
#include <GL/glew.h>

glm::mat4 Shader::projection_mat{};

Shader::Shader(const std::string &vs_path, const std::string &fs_path)
    : vs_path_(vs_path), fs_path_(fs_path)
{
    std::string vs_source = ReadFile(vs_path);
    std::string fs_source = ReadFile(fs_path);

    std::vector<std::pair<unsigned int, std::string>> shader_source = {
        {GL_VERTEX_SHADER, vs_source},
        {GL_FRAGMENT_SHADER, fs_source}
    };

    id_ = CreateShader(shader_source);
}

Shader::Shader(const std::string &vs_path, const std::string &gs_path, const std::string &fs_path)
    : vs_path_(vs_path), gs_path_(gs_path), fs_path_(fs_path)
{
    std::string vs_source = ReadFile(vs_path);
    std::string gs_source = ReadFile(gs_path);
    std::string fs_source = ReadFile(fs_path);

    std::vector<std::pair<unsigned int, std::string>> shader_source = {
        {GL_VERTEX_SHADER, vs_source},
        {GL_GEOMETRY_SHADER, gs_source},
        {GL_FRAGMENT_SHADER, fs_source}
    };

    id_ = CreateShader(shader_source);
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(id_));
}

void Shader::Bind() const
{
    GLCALL(glUseProgram(id_));
}

void Shader::Unbind() const
{
    GLCALL(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
    Bind();
    GLCALL(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    Bind();
    GLCALL(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    Bind();
    GLCALL(glUniform3f(GetUniformLocation(name), v0, v1 ,v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    Bind();
    GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2 &v)
{
    Bind();
    GLCALL(glUniform2f(GetUniformLocation(name), v[0], v[1]));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3 &v)
{
    Bind();
    GLCALL(glUniform3f(GetUniformLocation(name), v[0], v[1], v[2]));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4 &v)
{
    Bind();
    GLCALL(glUniform4f(GetUniformLocation(name), v[0], v[1], v[2], v[3]));
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
    Bind();
    GLCALL(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1ui(const std::string& name, unsigned int v0)
{
    Bind();
    GLCALL(glUniform1ui(GetUniformLocation(name), v0));
}

void Shader::SetUniformMatrix3f(const std::string& name, const glm::mat3 &matrix)
{
    Bind();
    GLCALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMatrix4f(const std::string& name, const glm::mat4 &matrix)
{
    Bind();
    GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

std::string Shader::ReadFile(const std::string& path)
{
    // stream of data used for reading input from a file
    std::ifstream t(path);
    // stream class to operate on strings
    std::stringstream buffer;
    // gets pointer to internal filebuf object
    buffer << t.rdbuf();
    return buffer.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    // Creates a shader object
    GLCALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();

    /*
    Replaces the source code in a shader object
    GLuint shader - Specifies the handle of the shader object whose source code is to be replaced.
 	GLsizei count - Specifies the number of elements in the string and length arrays.
 	const GLchar **string - Specifies an array of pointers to strings containing the source code to be loaded into the shader.
 	const GLint *length - Specifies an array of string lengths. (pass null to say strings are null terminated)
    */
    GLCALL(glShaderSource(id, 1, &src, nullptr));

    // Compiles a shader object
    GLCALL(glCompileShader(id));

    int result;
    // return a parameter from a shader object
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if(result == GL_FALSE)
    {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length];

        // Get info log for a shader object. Info log modified when shader is compiled.
        GLCALL(glGetShaderInfoLog(id, length, &length, message));
        
        std::string name = type == GL_VERTEX_SHADER ? "vertex" : (type == GL_FRAGMENT_SHADER ? "fragment" : "geometry");

        std::cout << "Failed to compile " << name << " shader!"<< std::endl;
        std::cout << message << std::endl;

        // Frees mem. Effectively undoes glCreateShader.
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::vector<std::pair<unsigned int, std::string>>& shader_source)
{
    // Creates a program to which shader objects can be attached
    GLCALL(unsigned int program = glCreateProgram());

    std::vector<unsigned int> shaders;
    for (const auto& [type, source] : shader_source)
    {
        GLCALL(unsigned int shader = CompileShader(type, source));
        // Attaches a shader object to a program object
        GLCALL(glAttachShader(program, shader));
        shaders.push_back(shader);
    }

    // Links a program object
    GLCALL(glLinkProgram(program));

    // Checks if exes contained in program can execute given the current OpenGL state
    GLCALL(glValidateProgram(program));

    // Frees mem. Effectively undoes glCreateShader.
    for(const auto &shader : shaders)
    {
        GLCALL(glDeleteShader(shader));
    }

    return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
    if(uniformLocationCache_.find(name) != uniformLocationCache_.end())
        return uniformLocationCache_[name];

    GLCALL(int location = glGetUniformLocation(id_, name.c_str()));
    if(location == -1)
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    uniformLocationCache_[name] = location;
    return location;
}
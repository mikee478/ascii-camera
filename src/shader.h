#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader(const std::string &vs_path, const std::string &fs_path);
    Shader(const std::string &vs_path, const std::string &gs_path, const std::string &fs_path);
    ~Shader();
    void Bind() const;
    void Unbind() const;

    void SetUniform1f(const std::string& name, float v0);
    void SetUniform2f(const std::string& name, float v0, float v1);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

    void SetUniform2f(const std::string& name, const glm::vec2 &v);
    void SetUniform3f(const std::string& name, const glm::vec3 &v);
    void SetUniform4f(const std::string& name, const glm::vec4 &v);

    void SetUniform1i(const std::string& name, int v0);
    void SetUniform1ui(const std::string& name, unsigned int v0);

    void SetUniformMatrix3f(const std::string& name, const glm::mat3 &matrix);
    void SetUniformMatrix4f(const std::string& name, const glm::mat4 &matrix);

    static glm::mat4 projection_mat;

private:
    std::string ReadFile(const std::string& path);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::vector<std::pair<unsigned int, std::string>>& shader_source);
    int GetUniformLocation(const std::string& name);

    unsigned int id_;
    std::string vs_path_;
    std::string gs_path_;
    std::string fs_path_;
    std::unordered_map<std::string, int> uniformLocationCache_;
};
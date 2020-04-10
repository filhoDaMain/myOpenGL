#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>    /* Hash table */
#include "glm/glm.hpp"

struct ShaderSourceCode
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    
public:
    Shader(const std::string& filepath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    
    /* Set Uniforms */
    /* ***************************************************************************** */
    
    /* 1 Integer */
    void SetUniform1i(const std::string& name, int v0);
    
    /* 4 floats (vec4) */
    void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
    
    /* Matrix 4x4 floats */
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    /* ***************************************************************************** */
    
private:
    ShaderSourceCode ParseShader(const std::string& filepath);
    
    unsigned int CompileShader(unsigned int type, const std::string& source);
    
    unsigned int CreateShader(const std::string& vertexShader,
                              const std::string& fragmentShader);
    
    int GetUniformLocation(const std::string& name);
};

#endif /* SHADER_H */


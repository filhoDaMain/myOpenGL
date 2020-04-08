#ifndef SHADER_H
#define SHADER_H

#include <string>

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
    
public:
    Shader(const std::string& filepath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    
    /* Set Uniforms */
    void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
    
    
private:
    ShaderSourceCode ParseShader(const std::string& filepath);
    
    unsigned int CompileShader(unsigned int type, const std::string& source);
    
    unsigned int CreateShader(const std::string& vertexShader,
                              const std::string& fragmentShader);
    
    int GetUniformLocation(const std::string& name);
};

#endif /* SHADER_H */


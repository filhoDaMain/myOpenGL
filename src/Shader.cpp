#include "Renderer.h"
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderSourceCode shaderSource = ParseShader(filepath);
    m_RendererID = CreateShader(shaderSource.vertexSource,
                                shaderSource.fragmentSource);
}

Shader::~Shader()
{
    GL_DEBUG( glDeleteProgram(m_RendererID) );
}

void Shader::Bind() const
{
    GL_DEBUG( glUseProgram(m_RendererID) );
}

void Shader::Unbind() const
{
    GL_DEBUG( glUseProgram(0) );
}


/* Set Uniforms */
/* ************************************************************************************ */

/* 1 Integer */
void Shader::SetUniform1i(const std::string& name, int v0)
{
    GL_DEBUG( glUniform1i(GetUniformLocation(name), v0) );
}

/* 4 floats (vec4) */
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GL_DEBUG( glUniform4f(GetUniformLocation(name), v0, v1, v2, v3) );
}

/* Matrix 4x4 floats */
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix){
    GL_DEBUG( glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &(matrix[0][0])) );
}
/* ************************************************************************************ */


int Shader::GetUniformLocation(const std::string& name)
{
    /**
     * If we have already populated the hash table with a location 
     * value for the key < name >, retrieve it now.
     * 
     * Otherwise, do the hard-work of finding location value and
     * populate it on the hash table prior to return it.
     */
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        /* Cache hit */
        return m_UniformLocationCache[name];    /* valid map */
    }
    
    /* Cache miss */
    GL_DEBUG( int location = glGetUniformLocation(m_RendererID, name.c_str()) );
    
    if (location == -1)
    {
        std::cout << "Warning: uniform " << name << " has location = -1" << std::endl;
    }

    /* Cache this value */
    m_UniformLocationCache[name] = location;
    return location;
}

ShaderSourceCode Shader::ParseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE        = -1,
        VERTEX      = 0,
        FRAGMENT    = 1
    };
    
    /* Shader type being currently read from file */
    ShaderType type = ShaderType::NONE;
    
    /* Open shader file */
    std::ifstream stream(filepath);
    std::string line;
    
    /**
     * Create two string streams, one per shader type.
     * 
     * ss[0]: VERTEX shader source code.
     * ss[1]: FRAGMENT shader source code
     */
    std::stringstream ss[2];
    
    /* Read all lines ( as long as getlline() > 0 )*/
    while(getline(stream, line))
    {
        /* Shader type delimiter */
        if (line.find("#shader") != std::string::npos)
        {   
            /**
             * Next token denotes if source code is for
             * a vertex or for a fragment shader
             */
            
            /* Vertex Shader */
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            
            /* Fragment Shader */
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }   
        }
        
        /* Shader source code */
        else
        {
            /* Directly mapping the shader type into ss[] index */
            ss[(int) type] << line << '\n';
        }
    }
    
    /**
     * Return both ss[] strings encapsulated in
     * a single ShaderSourceCode struct.
     */
    return { ss[0].str() /* vertex source */, 
             ss[1].str() /* fragment source */ };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const GLchar* src = source.c_str();
    
    /* Program shader: replace source code in a shader object */
    glShaderSource(id       /* shader (id) to program / replace source code */,
                   1        /* nr of elements in incomming strng && length arrays */,
                   &src     /* source code to be loaded into the shader */,
                   nullptr  /* array of src lengths */
                            /* (nullptr assumes every string is null terminated \0 ) */);
    
    /* Compile shader */
    glCompileShader(id);
    
    /**
     * Query shader status to check for compile errors
     * (if any)
     */
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS /* query compile status */, &status);
    
    /* Compilation FAILED */
    if (status == GL_FALSE)
    {
        /* Get length of error message */
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        
        /* Get the error message */
        char* emessage = (char *) alloca(length * sizeof(char));  /* stack allocation (auto freed) */
        /* NOTE: char emessage[length] assumes an infinite available stack -> g++ complains! */
        
        /* Get error message from shader log */
        glGetShaderInfoLog(id, length, &length, emessage /* log */);
        
        std::cout << "Failed to compile " <<
                ( type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT" ) << " shader!"
                << std::endl;
        
        /* Print error message */
        std::cout << emessage << std::endl;
        
        /* Delete shader object */
        glDeleteShader(id);
        return 0;
    }
    
    /* Return shader id */
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader,
                                  const std::string& fragmentShader)
{
    /* Crate an empty program object */
    GLuint program = glCreateProgram();
    
    /* Create a Vertex shader object - Compile */
    GLuint vshader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    
    /* Create a Fragment shader object - Compile */
    GLuint fshader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    /* Attach shader objects to a program object - Link */
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);
    
    /* Validate */
    glValidateProgram(program);
    
    /**
     * Now that shaders and program are compiled and linked,
     * we can delete shader objects to free the associated memory.
     */
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    
    return program;
}
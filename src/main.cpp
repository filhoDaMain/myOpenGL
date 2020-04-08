#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h> /* raise(SIGTRAP) */


#define SHADER_FILE_PATH "./res/shaders/Basic.shader"   /* Relative to project base dir */
#define _POSITION_ATTRIB_INDEX    0                     /* Index of position attribute */

struct ShaderSourceCode
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSourceCode parseShader(const std::string& filepath)
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

static GLuint compileShader(unsigned int type, const std::string& source)
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

static GLuint createShader(const std::string& vertexShader,
                        const std::string& fragmentShader)
{
    /* Crate an empty program object */
    GLuint program = glCreateProgram();
    
    /* Create a Vertex shader object - Compile */
    GLuint vshader = compileShader(GL_VERTEX_SHADER, vertexShader);
    
    /* Create a Fragment shader object - Compile */
    GLuint fshader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
     
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "myOpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);
    
    /* Glew Init */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    /* Print OpenGL version */
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
    
    /* ************************************************************** */
    /*    My Graphics Data (vertex positions and vertex indices)      */
    /* ************************************************************** */
    
    /* Vertex positions without duplicates */
    float positions[8] = {
        /* 0 */ -0.5f, -0.5f,
        /* 1 */  0.5f, -0.5f,
        /* 2 */  0.5f,  0.5f,
        /* 3 */ -0.5f,  0.5f
    };
    
    /* Using position indices to specify each triangle vertex position */
    unsigned int indices[6] = {
        /* 1st triangle */
        0, 1, 2,
        
        /* 2nd triangle */
        2, 3, 0
    };
    
    
    /* ************************************************************** */
    /*    Create a Vertex Array                                       */
    /* ************************************************************** */
    VertexArray va;     /* va is not bound yet! */
    
#if 0
    GLuint vao;
    GL_DEBUG( glGenVertexArrays(1, &vao) );
    GL_DEBUG( glBindVertexArray(vao) );
#endif

    /* ************************************************************** */
    /*    Create / Bind / Write 'positions' into a vertex-buffer      */
    /* ************************************************************** */
    VertexBuffer vb(positions, sizeof(positions));  /* vb stays bound */
      
    
    /* ************************************************************** */
    /*  Specify the data layout of vb and link it with Vertex Array   */
    /* ************************************************************** */
    
    /* Contains a vector of layouts */
    VertexBufferLayout layout;
    
    /* Inserts a new standard-layout of '2' components (x, y) in the vector */
    layout.PushStandardLayout<float>(2);
    
    /* Link vertex-buffer (vb) and its layouts with Vertex Array (va) */
    va.AddBuffer(vb, layout);            /* NOTE: va is still unbound */

    
    /* ************************************************************** */
    /*    Create / Bind / Write 'indices' into an index-buffer        */
    /* ************************************************************** */
    IndexBuffer ib(indices, (unsigned int)(sizeof(indices)/sizeof(unsigned int)));
    
    
    /* ************************************************************** */
    /*    Read / Compile / Link shaders source code;  Bind shader     */
    /* ************************************************************** */
    ShaderSourceCode shaderSource = parseShader(SHADER_FILE_PATH);
    GLuint shader = createShader(shaderSource.vertexSource,
                                  shaderSource.fragmentSource);
    GL_DEBUG( glUseProgram(shader) );
    
    
    /* ************************************************************** */
    /*    Define Uniforms to interchange color data                   */
    /* ************************************************************** */
    /* location = u_Color id */
    GL_DEBUG( GLint location = glGetUniformLocation(shader, "u_Color") );
    ASSERT(location != -1);
    
    
    /**
     * For Debug purposes,
     * unbind everything
     */
    va.Unbind();                    /* unbind Vertex Array */
    vb.Unbind();                    /* unbind vertex-buffer */
    ib.Unbind();                    /* unbind index-buffer */
    GL_DEBUG( glUseProgram(0) );    /* unbind shader */
    
    
    
    /* Animate red channel inside for loop */
    /* ------------------------------------------------------ */
    float red_ch = 0.0f;
    float increment = 0.05f;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear screen */
        GL_DEBUG( glClear(GL_COLOR_BUFFER_BIT) );
        
        
        va.Bind();                          /* Bind Vertex Array */
        ib.Bind();                          /* Bind index-buffer */
        GL_DEBUG( glUseProgram(shader) );   /* Bind shader */
        
        /* Update u_Color uniform (as a vec4) */
        GL_DEBUG( glUniform4f(location, red_ch, 1.0f, 0.0f, 1.0f) );
        
        
        /* Render */
        GL_DEBUG( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr) );
        
        /* Animate color change for next draw call */
        if (red_ch >= 1.0f)
        {
            increment = -0.05f;
        }
        else if (red_ch < 0)
        {
            increment =  0.05f;
        }
        red_ch += increment;
            
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GL_DEBUG( glDeleteProgram(shader) );
    glfwTerminate();
    return 0;
}
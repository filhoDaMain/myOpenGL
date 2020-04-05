#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define SHADER_FILE_PATH "./res/shaders/Basic.shader"    /* relative to project base dir */

static void _glClearError(void)
{
    /* Loop through all errors will clear them  */
    while (glGetError() != GL_NO_ERROR);
}

static void _glCheckError(void)
{
    while ( GLenum error = glGetError() )
    {
        std::cout << "OpenGL error:  error code (decimal) = " << error << std::endl;
    }
}

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
    
     /**
      * NOTE:
      * MacOS uses Legacy Profile as default for all created OpenGL context.
      * 
      * Set OpenGL version to 3.2 with glfwWindowHint()
      * before glfwCreateWindow()
      *  
      */
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
    
    /* Glew Init */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    }
    
    /* VAO - Vertex Array Object */
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    
    /* Print OpenGL version */
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
    /* Vertex positions without duplicates */
    float positions[8] = {
        /* 0 */ -0.5f, -0.5f,
        /* 1 */  0.5f, -0.5f,
        /* 2 */  0.5f,  0.5f,
        /* 3 */ -0.5f,  0.5f
    };
    
    /* Using position indices to specify each triangle vertex position */
    unsigned char indices[6] = {
        /* 1st triangle */
        0, 1, 2,
        
        /* 2nd triangle */
        2, 3, 0
    };
    
    /* Bind to buffer positions (4 vertices) */
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
     
    /* Change position component -> use index 0 */
    glVertexAttribPointer(0                 /* index of vertex attr to be modified (position) */,
                          2                 /* nr of components per this vertex attribute (x_pos, y_pos) */,
                          GL_FLOAT          /* data type of each vertex component */,
                          GL_FALSE          /* don't normalize (values are alreay [-1.0f, 1.0f] */,
                          2*sizeof(float)   /* byte offset between consecutive generic vertex attributes */,
                          0                 /* offset of 1st vertex component */
                          );
    
    /* Enable Position Vertex Attribute */
    glEnableVertexAttribArray(0 /* index of the generic vertex attribute to be enabled */);
    
   
    /* Bind to buffer indices (6 indices) */
    GLuint ibo;     /* Index Buffer Object */
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER /* index buffer type */, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER /* index buffer type */, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
    
    /* ************************************* */
    /*  Read shaders source code from file   */
    /* ************************************* */
    ShaderSourceCode shaderSource = parseShader(SHADER_FILE_PATH);
    
    /* ************************************** */
    /*  Compile and link shaders source code  */
    /* ************************************** */
    GLuint program = createShader(shaderSource.vertexSource,
                                  shaderSource.fragmentSource);
    
    /* ************************************** */
    /*      Install / Run program in GPU      */
    /* ************************************** */
    glUseProgram(program);
    
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear screen */
        glClear(GL_COLOR_BUFFER_BIT);
        
        _glClearError();
        glDrawElements(GL_TRIANGLES, 6 /* indices */, GL_UNSIGNED_BYTE, nullptr);
        _glCheckError();
        
        glDrawArrays(GL_TRIANGLES   /* type of primitive to render */,
                     0              /* starting index of enabled array (bounded buffer) */,
                     6              /* nr of indices on the array (nr of vertexes) */);
            
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}
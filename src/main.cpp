#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
        /* NOTE: char emmase[length] assumes an infinite available stack -> g++ complains! */
        
        /* Get error message from shader log */
        glGetShaderInfoLog(id, length, &length, emessage /* log */);
        
        std::cout << "Failed to compile " <<
                ( type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT" ) << " shader!"
                << std::endl;
        
        /* Print message */
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
      * Set OpenGL version to 3.2 by issuing glfwWindowHint() calls
      * before glfwCreateWindow()
      *  
      */
     
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
     
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
        
    /* Define my vertices */
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };
    
    /* Reserve  OpenGL Buffers */
    GLuint buffer;      /* buffer ID */
    glGenBuffers(1 /* how many buffers */, &buffer /* output ID */);
    
    /* Bind (select) a buffer to write */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(positions)  /* size (Bytes) of new data */,
                 positions          /* pointer to data being written into buffer */,
                 GL_STATIC_DRAW);
    
    /**
     * Vertex Attributes
     * 
     *  Attr    Attribute               Attribute
     *  Index   Name                    Description
     *  ---------------------------------------------------------------------------
     *   0      gl_Vertex               Position (vec4)
     *   1      gl_Normal               Normal (vec4)
     *   2      gl_Color                Primary color of vertex (vec4)
     *   3      gl_MultiTexCoord0       Texture coordinate of texture unit 0 (vec4)
     *   4      gl_MultiTexCoord1       Texture coordinate of texture unit 1 (vec4)
     *   5      gl_MultiTexCoord2       Texture coordinate of texture unit 2 (vec4)
     *   6      gl_MultiTexCoord3       Texture coordinate of texture unit 3 (vec4)
     *   7      gl_MultiTexCoord4       Texture coordinate of texture unit 4 (vec4)
     *   8      gl_MultiTexCoord5       Texture coordinate of texture unit 5 (vec4)
     *   9      gl_MultiTexCoord6       Texture coordinate of texture unit 6 (vec4)
     *  10      gl_MultiTexCoord7       Texture coordinate of texture unit 7 (vec4)
     *  11      gl_FogCoord             Fog Coord (float)
     * 
     */
    
    /* Change position component -> use index 0 */
    
    glVertexAttribPointer(0             /* index of vertex attr to be modified (position) */,
                          2             /* nr of components per this vertex attribute (x_pos, y_pos) */,
                          GL_FLOAT      /* data type of each vertex component */,
                          GL_FALSE      /* don't normalize (values are alreay [-1.0f, 1.0f] */,
                          2*sizeof(float)   /* byte offset between consecutive generic vertex attributes */,
                          0             /* offset of 1st vertex component */
                          );
    
    /* Enable Position Vertex Attribute */
    glEnableVertexAttribArray(0 /* index of the generic vertex attribute to be enabled */);
    
    
    /* ************************************ */
    /*  Create / write shaders source code  */
    /* ************************************ */
    
    /**
     * Vertex Shader source code - vertices position
     */
    /* ------------------------------------------------------------------- */
    std::string vsSource = 
            "#version 330 core\n"
            "\n"
            "layout(location = 0) in vec4 position;\n"
            "\n"
            "void main()\n"
            "{\n"
            "   gl_Position = position;\n"
            "}\n";
    /**
     *  layout(location = 0) in vec4 position 
     * creates a Vector4 position variable and assigns it ('in')
     * to the attribute index 0 (the attr index for positions)
     */
    
    
    /**
     * Fragment Shader source code - vertices colors
     */
    /* ------------------------------------------------------------------- */
    std::string fsSource = 
            "#version 330 core\n"
            "\n"
            "layout(location = 0) out vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
            "}\n";
    /* Color = RED [ Normalized (r, g, b, alpha) ] */
    
    
    /* ************************************** */
    /*  Compile and link shaders source code  */
    /* ************************************** */
    GLuint program = createShader(vsSource, fsSource);
    
    /* Install / Run program in GPU */
    glUseProgram(program);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear screen */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES   /* type of primitive to render */,
                     0              /* starting index of enabled array (bounded buffer) */,
                     3              /* nr of indices on the array (nr of vertexes) */);
        
        
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
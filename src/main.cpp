#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


int main(void)
{
     GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
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
    
    /* Print OpenGL version */
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
    /* Moder OpenGL */
    /* --------------------------------------------- */
    
    /* Define my vertices */
    float positions[6] = {
        -0.5f, -0.5f,
        -0.0f,  0.5f,
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
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES   /* type of primitive to render */,
                     0              /* starting index of enabled array (bounded buffer) */,
                     3              /* nr of indices on the array (nr of vertexes) */);
        
        #if 0
        /* Legacy OpenGL */
        /* --------------------------------------------- */
        /* Triangle */
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);   // vertex 0
        glVertex2f(-0.0f,  0.5f);   // vertex 1
        glVertex2f( 0.5f, -0.5f);   // vertex 2
        glEnd();
        /* --------------------------------------------- */
         #endif
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
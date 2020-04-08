#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h> /* raise(SIGTRAP) */

#define SHADER_FILE_PATH "./res/shaders/Basic.shader"   /* Relative to project base dir */

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
    /* Read / Compile / Link shaders;  Set shaders Uniform for color  */
    /* ************************************************************** */
    Shader shader(SHADER_FILE_PATH);
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0);
    
    
    
    /**
     * For Debug purposes,
     * unbind everything
     */
    va.Unbind();        /* unbind Vertex Array */
    vb.Unbind();        /* unbind vertex-buffer */
    ib.Unbind();        /* unbind index-buffer */
    shader.Unbind();    /* unbind shader */
     
    
    /* Animate red channel inside for loop */
    /* ------------------------------------------------------ */
    float red_ch = 0.0f;
    float increment = 0.05f;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear screen */
        GL_DEBUG( glClear(GL_COLOR_BUFFER_BIT) );
        
        
        va.Bind();          /* Bind Vertex Array */
        ib.Bind();          /* Bind index-buffer */
        shader.Bind();      /* Bind shader */
                
        /* Update u_Color uniform (as a vec4) */
        shader.SetUniform4f("u_Color", red_ch, 1.0f, 0.0f, 1.0f); 
        
        
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
    
    glfwTerminate();
    return 0;
}
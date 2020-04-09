#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h> /* raise(SIGTRAP) */

/**
 * NOTE:
 * 
 * File Paths are all defined relative to project base directory 
 */

/* Shaders */
#define SHADER_FILE_PATH    "./res/shaders/Basic.shader"

/* Textures */
#define TEXTURE_MORIS_PATH  "./res/textures/Moris_28x20cm.png"

/* Uniforms */
#define MY_COLOR_UNIFORM4   "u_Color"   /* vec4 uniform name for color */

/* Texture slots */
#define TEXTURE_MORIS_SLOT  0           /* The slot where our texture is bound */

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
    /*    My Graphics Data (vertexes and vertex indices)              */
    /* ************************************************************** */
    
    /* SEE TABLE doc/VertexAttributes.txt */
    
    float vertexAttribs[16] = {
        /*      |--- Vector pos ----|----- Texture coordinate boundaries ----| */
        /* 0 */     -0.5f, -0.5f,       0.0f, 0.0f  /* bottom left corner */,
        /* 1 */      0.5f, -0.5f,       1.0f, 0.0f  /* bottom right corner */,
        /* 2 */      0.5f,  0.5f,       1.0f, 1.0f  /* upper right corner */,
        /* 3 */     -0.5f,  0.5f,       0.0f, 1.0f  /* upper left corner */
        /*      |- Attrib index 0 --|-------------- Attrib index 1 ----------| */
        /*      |-------------- S T R I D E ---------------------------------| */
    };
    
    /* Using vertexAttribs indices to specify each triangle vertex */
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
    /*    Create / Bind / Write 'vertexAttribs' into a vertex-buffer  */
    /* ************************************************************** */
    VertexBuffer vb(vertexAttribs, sizeof(vertexAttribs));  /* vb stays bound */
      
    
    /* ************************************************************** */
    /*  Specify the data layout of vb and link it with Vertex Array   */
    /* ************************************************************** */
    
    /* Contains a vector of layouts */
    VertexBufferLayout layout;
    
    /**
     * Layouts are inserted by vertex attribute index order.
     * 
     * This means, 1st inserted layout defines vertex attrib index 0 - position;
     * next layout defines attrib index 1 - normal;
     * and so on...
     * (SEE TABLE doc/VertexAttributes.txt)
     * 
     * Each time a layout is inserted, stride size (Bytes) is properly updated.
     *  
     */
    layout.PushVertexAttribLayout<float>(2);    /* Vertex attrib index 0: as '2' floats */
    layout.PushVertexAttribLayout<float>(2);    /* Vertex attrib index 1: as '2' floats */
    
    /**
     * Link vertex-buffer (vb) and its layouts with Vertex Array (va).
     */
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
    
    
    /* ************************************************************** */
    /*    Create a Renderer object                                    */
    /* ************************************************************** */
    Renderer renderer;
    
    
    /* ************************************************************** */
    /*    Textures                                                    */
    /* ************************************************************** */
    Texture texture(TEXTURE_MORIS_PATH);
    texture.Bind(TEXTURE_MORIS_SLOT);
    shader.SetUniform1i("u_Texture", TEXTURE_MORIS_SLOT);
    
    
    /**
     * For Debug purposes,
     * unbind everything
     */
    va.Unbind();        /* unbind Vertex Array */
    vb.Unbind();        /* unbind vertex-buffer */
    ib.Unbind();        /* unbind index-buffer */
    shader.Unbind();    /* unbind shader */
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear screen */
        renderer.Clear();
        
        /* Shader has to be bound prior to Update a uniform */
        shader.Bind();
        
        /* Draw Call */
        renderer.Draw(va, ib, shader); 
      
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
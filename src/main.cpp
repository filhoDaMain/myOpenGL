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
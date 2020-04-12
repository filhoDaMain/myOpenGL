#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
#define TEXTURE_PATH__MORIS "./res/textures/Moris_28x20cm.png"

/* Uniforms */
#define MY_COLOR_UNIFORM4   "u_Color"   /* vec4 uniform name for color */

/* Texture slots */
#define TEXTURE_SLOT__MORIS  0          /* The slot where our texture is bound */

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
     
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
    
    /* SEE doc/VertexAttributes.txt */
    
    float vertexAttribs[16] = {
        /*      |------ Vertex pos ------|----- Texture coordinate boundaries -----| */
        /* 0 */     -0.5f, -0.5f,            0.0f, 0.0f  /* bottom left corner */,
        /* 1 */      0.5f, -0.5f,            1.0f, 0.0f  /* bottom right corner */,
        /* 2 */      0.5f,  0.5f,            1.0f, 1.0f  /* upper right corner */,
        /* 3 */     -0.5f,  0.5f,            0.0f, 1.0f  /* upper left corner */
        /*      |- layout(location = 0) -|--------- layout(location = 1) ----------| */
        /*      |------------------------ S T R I D E -----------------------------| */
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
     * Layouts are inserted in a vector by ascending order of layout
     * position index.
     * 
     * This means, 1st inserted layout is stored in (location = 0).
     * Next one, is inserted in (location = 1), and so on.
     * 
     * When writing a Shader, one should take notice of the order these layouts
     * were inserted to correctly reference them.
     * 
     * Example:
     * layout(location = 0) in vec4 position;
     * layout(location = 1) in vec2 texCoord;
     * 
     * Should only be applied if 1st inserted layout represents vertex positions
     * and the 2nd inserted layout represents texture coordinates.
     * 
     * Each time a layout is inserted, stride size (Bytes) is auto increased.
     *  
     */
    layout.PushVertexAttribLayout<float>(2);    /* (location = 0) as '2' floats */
    layout.PushVertexAttribLayout<float>(2);    /* (location = 1) as '2' floats */
    
    /**
     * Link vertex-buffer (vb) and its layouts with Vertex Array (va).
     */
    va.AddBuffer(vb, layout);            /* NOTE: va is still unbound */

    
    /* ************************************************************** */
    /*    Create / Bind / Write 'indices' into an index-buffer        */
    /* ************************************************************** */
    IndexBuffer ib(indices, (unsigned int)(sizeof(indices)/sizeof(unsigned int)));
    
    
    /* ************************************************************** */
    /*    Create a Renderer object                                    */
    /* ************************************************************** */
    Renderer renderer;
    
    
    /* ************************************************************** */
    /*    Textures                                                    */
    /* ************************************************************** */
    Texture texture(TEXTURE_PATH__MORIS);
    texture.Bind(TEXTURE_SLOT__MORIS);
    
    
    /* ************************************************************** */
    /*    MVP Matrixes                                                */
    /* ************************************************************** */
    
    /* ***************************************************** */
    /* Projection Matrix (proj) - translates Texture coords  */
    /* ***************************************************** */
    glm::mat4 proj = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, -1.0f, 1.0f);
    
    
    /* ************************************************* */
    /*  View Matrix (view) - translates Camera position  */
    /* ************************************************* */
    glm::mat4 view;
    
    /**
     * Move Camera to Right.
     * 
     * The effect of moving a Camera to right is got by moving the entire world
     * to left (oposite movement in all directions) by the same amount of world
     * units.
     * 
     * We achieve this by translating an identity matrix 4x4 and assigning it
     * to our view matrix.
     */
    glm::mat4 identity(1.0f);       /* identity matrix */
    //view = glm::translate(identity, glm::vec3(-1, 0, 0));
    view = identity;
    
    /* ************************************************** */
    /*  Model Matrix (model) - translates Model position  */
    /* ************************************************** */
    glm::mat4 model;
    
    /**
     * Moving the model to a certain direction effect is obtained by translating
     * an identity matrix the amount of units we want in a given direction and
     * applying the result to the model matrix. 
     */
    //model = glm::translate(identity, glm::vec3(0.5, 1, 0));
    model = identity;
    
    
    /* ************************************************************ */
    /*  Model View Projection Matrix (mvp) - what goes into Shader  */
    /* ************************************************************ */
    glm::mat4 mvp = proj * view * model;    /* Matrix multiplication order matters!*/
    
    
    
    /* ************************************************************** */
    /* Shaders: Set Uniforms                                          */
    /* ************************************************************** */
    Shader shader(SHADER_FILE_PATH);
    shader.Bind();
    shader.SetUniform1i("u_Texture", TEXTURE_SLOT__MORIS);
    shader.SetUniformMat4f("u_MVP", mvp);
    
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    
    glm::vec3 translation(0.0f, 0.0f, 0.0f);
    
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
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
#if 0
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#endif
        
        /* Shader has to be bound prior to Update a uniform */
        shader.Bind();
        
        model = glm::translate(identity, translation);
        mvp = proj * view * model;    /* Matrix multiplication order matters!*/
        shader.SetUniformMat4f("u_MVP", mvp);
        
        /* Draw Call */
        renderer.Draw(va, ib, shader); 
      
        /* Simple Window */
        {
#if 0
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);
#endif
            
            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            
            ImGui::SliderFloat("float", &translation.x, -1.5f, 1.5f);            // Edit 1 float using a slider from 0.0f to 1.0f
            
#if 0
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
#endif
            
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            //ImGui::End();
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
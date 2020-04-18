#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>

/* My Tests Framework */
/* *************************************************************** */
#include "Test.h"
#include "tests/TestClearColor.h"       /* Change Clear Color */
#include "tests/TestTexture2D.h"        /* 2D Texture Manipulation */
#include "tests/TestBatchRendering.h"   /* Batch Rendering */
/* *************************************************************** */


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
    /*  ImGui Init                                                    */
    /* ************************************************************** */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    
    /* ************************************************************** */
    /*  My Tests Framework                                            */
    /* ************************************************************** */
    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu; /* Current ImGui Screen */
    
    /* Tests / Examples: */
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
    testMenu->RegisterTest<test::TestBatchRendering>("Batch Rendering");
    
    
    /* ************************************************************** */
    /*  Renderer object                                               */
    /* ************************************************************** */
    Renderer renderer;
    
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        GL_DEBUG( glClearColor(0.0f, 0.0f, 0.0f, 1.0f) );
        
        /* Clear screen */
        renderer.Clear();
        
        /* ImGui Frame Init */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        /* Render Tests */
        /* ------------------------------------------------------------ */
        if (currentTest != nullptr)
        {
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();
            ImGui::Begin("Test");
            if ( currentTest != testMenu && ImGui::Button("<- BACK") )
            {
                delete currentTest;
                currentTest = testMenu;
            }
            
            currentTest->OnImGuiRender();
            ImGui::End();
        }
        /* ------------------------------------------------------------ */
        
        /* ImGui Render Call */
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
        
    }   /* while rendering loop */
    
    delete currentTest;
    if (currentTest != testMenu)
    {
        delete testMenu;
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
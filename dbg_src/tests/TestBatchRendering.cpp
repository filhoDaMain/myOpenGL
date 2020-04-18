#include "TestBatchRendering.h"

/* Shaders */
#define SHADER_FILE_PATH    "./res/shaders/ColorShader.shader"

/* Uniforms */
#define MY_COLOR_UNIFORM4_NAME          "u_Color"   /* Uniform name for color */
#define MVP_MATRIX_UNIFORMMAT4F_NAME    "u_MVP"     /* Uniform name for MVP matrix */

namespace test
{
    TestBatchRendering::TestBatchRendering()
        :   m_Translation(0.0f, 0.0f, 0.0f), 
            m_Proj(glm::ortho(-2.5f, 2.5f, -2.5f, 2.5f, -1.0f, 1.0f)),
            m_View(glm::mat4(1.0f)) 
    {
        
        /* ************************************************************** */
        /*    My Graphics Data (vertexes and vertex indices)              */
        /* ************************************************************** */
    
        /* SEE doc/VertexAttributes.txt */
    
        float vertexAttribs[16] = {
            /*      |------ Vertex pos ------| */
            /* 0 */        -1.5f, -0.5f,
            /* 1 */        -0.5f, -0.5f,
            /* 2 */        -0.5f,  0.5f,
            /* 3 */        -1.5f,  0.5f,
            
            /* 4 */         0.5f, -0.5f,
            /* 5 */         1.5f, -0.5f,
            /* 6 */         1.5f,  0.5f,
            /* 7 */         0.5f,  0.5f,
            /*      |- layout(location = 0) -| */
            /*      |------ S T R I D E -----| */
        };
    
        /* Using vertexAttribs indices to specify each triangle vertex */
        unsigned int indices[12] = {
            /* 1st triangle */
            0, 1, 2,
            /* 2nd triangle */
            2, 3, 0,
            
            /* 3rd triangle */
            4, 5, 6,
            /* 4th triangle */
            6, 7, 4          
        };
    
    
        /* ************************************************************** */
        /*    Create a Vertex Array                                       */
        /* ************************************************************** */
        m_VAO = std::make_unique<VertexArray>();    /* va is not bound yet! */
    

        /* ************************************************************** */
        /*    Create / Bind / Write 'vertexAttribs' into a vertex-buffer  */
        /* ************************************************************** */
        m_VBO = std::make_unique<VertexBuffer>(vertexAttribs,
                                               sizeof(vertexAttribs));
      
    
        /* ************************************************************** */
        /* Specify the data layout of m_VBO and link it with Vertex Array */
        /* ************************************************************** */
    
        /* Contains a vector of layouts */
        VertexBufferLayout layout;
        layout.PushVertexAttribLayout<float>(2);    /* (location = 0) as '2' floats */
         m_VAO->AddBuffer(*m_VBO, layout);          /* NOTE: mVAO is still unbound */

    
        /* ************************************************************** */
        /*    Create / Bind / Write 'indices' into an index-buffer        */
        /* ************************************************************** */
         unsigned int indicesCount =
                    (unsigned int)(sizeof(indices)/sizeof(unsigned int));
         
         m_IBO = std::make_unique<IndexBuffer>(indices, indicesCount);
    
    
        /* ************************************************************** */
        /*    Create a Renderer object                                    */
        /* ************************************************************** */
        Renderer renderer;
    
    
        /* ************************************************************** */
        /*    MVP Matrixes                                                */
        /* ************************************************************** */
        glm::mat4 identity(1.0f);       /* identity matrix */
        glm::mat4 model = identity;
        glm::mat4 mvp = m_Proj * m_View * model;
    
    
        /* ************************************************************** */
        /* Shaders: Set Uniforms                                          */
        /* ************************************************************** */
        m_Shader = std::make_unique<Shader> (SHADER_FILE_PATH);
        m_Shader->Bind();
        m_Shader->SetUniform4f(MY_COLOR_UNIFORM4_NAME, 1.0f, 0.0f, 0.0f, 1.0f);
        m_Shader->SetUniformMat4f(MVP_MATRIX_UNIFORMMAT4F_NAME, mvp);
        
    }
    
    TestBatchRendering::~TestBatchRendering()
    {
        
    }
    
    void TestBatchRendering::OnUpdate(float deltaTime)
    {
        
    }
    
    void TestBatchRendering::OnRender()
    {
        Renderer renderer;
        glm::mat4 identity(1.0f);   /* identity matrix */
        glm::mat4 model;
        glm::mat4 mvp;
        
        GL_DEBUG( glClearColor(0.0f, 0.0f, 0.0f, 1.0f) );
        GL_DEBUG( glClear(GL_COLOR_BUFFER_BIT) );
        
        mvp = m_Proj * m_View * model;
            
        /* Shader has to be bound prior to Update a uniform */
        m_Shader->Bind();
        
        /* My Draw Call */
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader); 
    }
    
    void TestBatchRendering::OnImGuiRender()
    {
        
    }
    
}
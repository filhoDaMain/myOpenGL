#include "TestTexture2D.h"
#include "imgui/imgui.h"

/* Shaders */
#define SHADER_FILE_PATH        "./res/shaders/Basic.shader"

/* Textures */
#define TEXTURE_PATH__MORIS     "./res/textures/Moris_28x20cm.png"

/* Uniforms */
#define MY_COLOR_UNIFORM4_NAME          "u_Color"   /* Uniform name for color */
#define MVP_MATRIX_UNIFORMMAT4F_NAME    "u_MVP"     /* Uniform name for MVP matrix */
#define TEXTURE_SLOT_UNIFORM1I_NAME     "u_Texture" /* Uniform name for slot integer */

/* Texture slots */
#define TEXTURE_SLOT__MORIS  0          /* The slot where our texture is bound */

namespace test
{
    
    TestTexture2D::TestTexture2D()
        :   m_TranslationA(0.0f, 0.0f, 0.0f), m_TranslationB(0.0f, 0.5f, 0.0f),
            m_Proj(glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, -1.0f, 1.0f)),
            m_View(glm::mat4(1.0f)) 
    {
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
         * Link vertex-buffer (m_VBO) and its layouts with Vertex Array (m_VAO).
         */
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
        /*    Textures                                                    */
        /* ************************************************************** */
        m_Texture = std::make_unique<Texture>(TEXTURE_PATH__MORIS);
        m_Texture->Bind();
    
    
        /* ************************************************************** */
        /*    MVP Matrixes                                                */
        /* ************************************************************** */
        
        /**
         * m_Proj:  Projection Matrix   - trnaslates Texture coords
         * m_View:  View Matrix         - translates Camera position
         * modeL:   Model Matrix        - translates Model position
         */
        
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
        //m_View = glm::translate(identity, glm::vec3(-1, 0, 0));
    
        
        /**
         * Moving the model to a certain direction effect is obtained by translating
         * an identity matrix the amount of units we want in a given direction and
         * applying the result to the model matrix. 
         */
        //glm::mat4 model; = glm::translate(identity, glm::vec3(0.5, 1, 0));
        glm::mat4 model = identity;
    
    
        /* ************************************************************ */
        /*  Model View Projection Matrix (mvp) - what goes into Shader  */
        /* ************************************************************ */
        glm::mat4 mvp = m_Proj * m_View * model;    /* Matrix multiplication order matters!*/
    
    
        /* ************************************************************** */
        /* Shaders: Set Uniforms                                          */
        /* ************************************************************** */
        m_Shader = std::make_unique<Shader> (SHADER_FILE_PATH);
        m_Shader->Bind();
        m_Shader->SetUniform1i(TEXTURE_SLOT_UNIFORM1I_NAME, TEXTURE_SLOT__MORIS);
        m_Shader->SetUniformMat4f(MVP_MATRIX_UNIFORMMAT4F_NAME, mvp);
    }
    
    TestTexture2D::~TestTexture2D()
    {
        
    }
    
    void TestTexture2D::OnUpdate(float deltaTime)
    {
        
    }
    
    void TestTexture2D::OnRender()
    {
        Renderer renderer;
        glm::mat4 identity(1.0f);       /* identity matrix */
        glm::mat4 model;
        glm::mat4 mvp;
        
        GL_DEBUG( glClearColor(0.0f, 0.0f, 0.0f, 1.0f) );
        GL_DEBUG( glClear(GL_COLOR_BUFFER_BIT) );
        
        m_Texture->Bind();
        
        /* Model A */
        {
            model = glm::translate(identity, m_TranslationA);
            mvp = m_Proj * m_View * model;
            
            /* Shader has to be bound prior to Update a uniform */
            m_Shader->Bind();
            m_Shader->SetUniformMat4f(MVP_MATRIX_UNIFORMMAT4F_NAME, mvp);
        
            /* My Draw Call */
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader); 
        }
        
        /* Model B */
        {
            model = glm::translate(identity, m_TranslationB);
            mvp = m_Proj * m_View * model;
            
            /* Shader has to be bound prior to Update a uniform */
            m_Shader->Bind();
            m_Shader->SetUniformMat4f(MVP_MATRIX_UNIFORMMAT4F_NAME, mvp);
        
            /* My Draw Call */
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader); 
        }
    }
    
    void TestTexture2D::OnImGuiRender()
    {
        /* Apply Slider's value to m_TranslationA matrix's X value */
        ImGui::SliderFloat("Moris A - X position", &m_TranslationA.x, -1.5f, 1.5f);
        
        /* Apply Slider's value to m_TranslationB matrix's X value */
        ImGui::SliderFloat("Moris B - X position", &m_TranslationB.x, -1.5f, 1.5f);
    }
}
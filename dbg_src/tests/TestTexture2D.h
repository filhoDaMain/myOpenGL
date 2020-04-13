#ifndef TESTTEXTURE_H
#define TESTTEXTURE_H

#include "Test.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

namespace test
{
    
    class TestTexture2D : public Test
    {
    private:
        /**
         * Translation matrix to apply a new model translation
         * inside the Render while loop.
         */
        glm::vec3 m_TranslationA, m_TranslationB;
        
        glm::mat4 m_Proj, m_View;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;
        
    public:
        TestTexture2D();
        ~TestTexture2D();
        
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
    
}

#endif /* TESTTEXTURE_H */


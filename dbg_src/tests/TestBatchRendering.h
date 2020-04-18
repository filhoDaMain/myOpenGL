#ifndef TESTBATCHRENDERING_H
#define TESTBATCHRENDERING_H

#include "Test.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
//#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

namespace test
{
    
    class TestBatchRendering : public Test
    {
    private:
        glm::vec3 m_Translation;
        glm::mat4 m_Proj, m_View;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_Shader;
        
    public:
        TestBatchRendering();
        ~TestBatchRendering();
        
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
    
}

#endif /* TESTBATCHRENDERING_H */


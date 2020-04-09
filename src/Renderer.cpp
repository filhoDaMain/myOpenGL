#include "Renderer.h"
#include <iostream>

void _glClearError(void)
{
    /* Loop through all errors will clear them  */
    while (glGetError() != GL_NO_ERROR);
}

bool _glLogCall(const char* function, const char* srcfile, int line)
{
    while ( GLenum error = glGetError() )
    {
        std::cout << "OpenGL error code = " << error << " (decimal)" << std::endl;
        std::cout << "  File: " << srcfile << std::endl;
        std::cout << "  Line: " << line << std::endl;
        return false;
    }
    
    return true;
}

/**
 * The vertex type is hardcoded as GL_UNSIGNED_INT !
 * That must be the indices[] type for now!!!
 * 
 * @param va
 * @param ib
 * @param shader
 */
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();          /* Bind Vertex Array */
    ib.Bind();          /* Bind index-buffer */
    shader.Bind();      /* Bind shader */
    
    /* Render */
    GL_DEBUG( glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr) );
}

void Renderer::Clear() const
{
    /* Clear screen */
    GL_DEBUG( glClear(GL_COLOR_BUFFER_BIT) );
}

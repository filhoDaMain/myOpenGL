#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include <stdint.h>     /* uintptr_t */

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

VertexArray::VertexArray()
{
    GL_DEBUG( glGenVertexArrays(1, &m_RendererID) );
    //GL_DEBUG( glBindVertexArray(m_RendererID) );
}

VertexArray::~VertexArray()
{
    GL_DEBUG( glDeleteVertexArrays(1, &m_RendererID) );
}

void VertexArray::Bind() const
{
    GL_DEBUG( glBindVertexArray(m_RendererID) );
}

void VertexArray::Unbind() const
{
    GL_DEBUG( glBindVertexArray(0) );
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    this->Bind();
    vb.Bind();
    
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    GLvoid* offset_ptr;
    
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        offset_ptr = INT2VOIDP(offset); /* avoid compiler warnings */

        GL_DEBUG( glEnableVertexAttribArray(i) );
        GL_DEBUG( glVertexAttribPointer(
                        i,
                        element.n_components,
                        element.type,
                        element.is_normalized,
                        layout.GetStride(),
                        offset_ptr)
                );
        offset += element.n_components * VertexBufferElement::GetSizeOfType(element.type);
    }
    

}
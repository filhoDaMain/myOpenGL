#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    /* Generate and Bind index-buffer */
    GL_DEBUG( glGenBuffers(1, &m_RendererID) );
    GL_DEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
    
    /* Write data in index-buffer */
    GL_DEBUG( glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW) );
}

IndexBuffer::~IndexBuffer()
{
    GL_DEBUG( glDeleteBuffers(1, &m_RendererID ) );
}

void IndexBuffer::Bind() const
{
    GL_DEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
}

void IndexBuffer::Unbind() const
{
    GL_DEBUG( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}
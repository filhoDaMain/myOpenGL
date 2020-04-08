#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    /* Generate and Bind vertex-buffer */
    GL_DEBUG( glGenBuffers(1, &m_RendererID) );
    GL_DEBUG( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );
    
    /* Write data in vertex-buffer */
    GL_DEBUG( glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW) );
}

VertexBuffer::~VertexBuffer()
{
    GL_DEBUG( glDeleteBuffers(1, &m_RendererID ) );
}

void VertexBuffer::Bind() const
{
    GL_DEBUG( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );
}

void VertexBuffer::Unbind() const
{
    GL_DEBUG( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}
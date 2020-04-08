#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
    : m_Stride(0)
{
    
}

VertexBufferLayout::~VertexBufferLayout()
{
    
}


/* Template Specializations */

// FLOAT
template<>
void VertexBufferLayout::PushStandardLayout<float>(unsigned int n_components)
{
    m_Elements.push_back({GL_FLOAT, n_components, GL_FALSE});
    m_Stride += sizeof(GLfloat) * n_components;
}

// UNSIGNED INT
template<>
void VertexBufferLayout::PushStandardLayout<unsigned int>(unsigned int n_components)
{
    m_Elements.push_back({GL_UNSIGNED_INT, n_components, GL_FALSE});
    m_Stride += sizeof(GLuint) * n_components; 
}

// UNSIGNED CHAR
template<>
void VertexBufferLayout::PushStandardLayout<unsigned char>(unsigned int n_components)
{
    m_Elements.push_back({GL_UNSIGNED_BYTE, n_components, GL_TRUE });
    m_Stride += sizeof(GLbyte) * n_components; 
}

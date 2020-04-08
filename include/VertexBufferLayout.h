#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include "GL/glew.h"
#include <vector>
#include "Renderer.h"


struct VertexBufferElement
{
    unsigned int    type;
    unsigned int    n_components;
    unsigned char   is_normalized;
    
    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:          return sizeof(GLfloat);
            case GL_UNSIGNED_INT:   return sizeof(GLint);
            case GL_UNSIGNED_BYTE:  return sizeof(GLbyte);
        }
        ASSERT(false);
        return -1;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement>    m_Elements;
    unsigned int                        m_Stride;
    
public:
    VertexBufferLayout();
    ~VertexBufferLayout(); 
    
    /* Template specializations can be found in VertexBufferLayout.cpp */
    
    /**
     * Inserts a VertexBufferEllement (Standard Layout) on a vector of 
     * elements where the number of components per generic vertex attribute 
     * is the only parametrized attribute.
     * 
     * @param n_components  How many components per 1 attribute
     * Example: If 1 attribute encodes (x_pos, y_pos) => n_components = 2
     */
    template<typename T>
    void PushStandardLayout(unsigned int n_components)
    {
        //static_assert(false, "No template specialization found for requested type");
    }
    
    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    
    inline unsigned int GetStride() const { return m_Stride; }
};

#endif /* VERTEXBUFFERLAYOUT_H */


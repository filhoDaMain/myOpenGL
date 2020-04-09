#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"

/* We choose to not include VertexBufferLayout to avoid mutual includes! */
/* Mutual inclusions: Render.h -> VertexArray.h -> VertexBufferLayout.h -> Renderer.h */

class VertexBufferLayout;   /* Declared in VertexBufferLayout.h */

class VertexArray
{
private:
    unsigned int m_RendererID;
    
public:
    VertexArray();
    ~VertexArray();
    
    void Bind() const;
    void Unbind() const;
    
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};

#endif /* VERTEXARRAY_H */


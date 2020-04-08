#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;       /* how many indices */
    
public:
    IndexBuffer(const unsigned int* data, unsigned int count /* how many indices */);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
    
    inline unsigned int GetCount() const { return m_Count; }
};

#endif /* INDEXBUFFER_H */


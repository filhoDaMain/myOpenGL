#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <signal.h> /* raise(SIGTRAP) */
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


/* raise(SIGTRAP) will cause the program execution to break (POSIX) */
#define ASSERT(x) if(!(x)) raise(SIGTRAP);  

#define GL_DEBUG(x) _glClearError();\
    x;\
    ASSERT(_glLogCall(#x /* func name as string */, __FILE__, __LINE__))

extern void _glClearError(void);
extern bool _glLogCall(const char* function, const char* srcfile, int line);

class Renderer
{
private:
    
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
};

#endif /* RENDERER_H */


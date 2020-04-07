#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>


/* raise(SIGTRAP) will cause the program execution to break (POSIX) */
#define ASSERT(x) if(!(x)) raise(SIGTRAP);  

#define GL_DEBUG(x) _glClearError();\
    x;\
    ASSERT(_glLogCall(#x /* func name as string */, __FILE__, __LINE__))

extern void _glClearError(void);
extern bool _glLogCall(const char* function, const char* srcfile, int line);

#endif /* RENDERER_H */

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

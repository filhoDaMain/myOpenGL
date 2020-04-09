#include "Texture.h"
#include "vendor/stb/stb_image.h"

Texture::Texture(const std::string& path)
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
      m_Width(0), m_Height(0), m_bpp(0)
{
    /**
     * Flip Texture vertically because OpenGL considers
     * the bottom left pixel to be the pixel (0, 0)
     */
    stbi_set_flip_vertically_on_load(1);
    
    /* Load Image */
    m_LocalBuffer = stbi_load(  path.c_str(),
                                &m_Width, &m_Height,
                                &m_bpp,
                                4 /* desired bpp (rgba)*/);
    
    /* Generate and Bind an OpenGL Texture */
    GL_DEBUG( glGenTextures(1, &m_RendererID) );
    GL_DEBUG( glBindTexture(GL_TEXTURE_2D, m_RendererID) );
    
    /* Minification Filter */
    GL_DEBUG( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
    
    /* Magnification Filter */
    GL_DEBUG( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
    
    /* Horizontal Wrap - Clamp -> Don't extend the area */
    GL_DEBUG( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
    
    /* Vertical Wrap - Clamp -> Don't extend the area */
    GL_DEBUG( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
    
    /* Specify a 2D Texture Image */
    GL_DEBUG( glTexImage2D( GL_TEXTURE_2D,
                            0, GL_RGBA8 /* Internal Format: How OpenGL will store the data */,
                            m_Width, m_Height,
                            0,
                            GL_RGBA     /* Format: How Texture data (m_LocalBuffer) is encoded */,
                            GL_UNSIGNED_BYTE,
                            m_LocalBuffer) );
    
    /* Unbind Texture */
    GL_DEBUG( glBindTexture(GL_TEXTURE_2D, 0 ) );
    
    if (m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::~Texture()
{
    GL_DEBUG( glDeleteTextures(1, &m_RendererID) );
}
        
void Texture::Bind(unsigned int slot /* = 0 */) const
{
    GL_DEBUG( glActiveTexture(GL_TEXTURE0 + slot) );
    GL_DEBUG( glBindTexture(GL_TEXTURE_2D, m_RendererID) );
}

void Texture::Unbind() const
{
    GL_DEBUG( glBindTexture(GL_TEXTURE_2D, 0 ) );
}
#include "frontend/gfx/texture.h"
#include "sdl/surface.h"

#include <GL/glew.h>

namespace Gfx {

Texture::Texture(u32 id, s32 width, s32 height) :
    m_id{id},
    m_width{width},
    m_height{height}
{}

u32 Texture::GetCurrentBound()
{
    int id;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);

    return id;
}

void Texture::Destroy(u32 id)
{
    glDeleteTextures(1, &id);
}

void Texture::Create()
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::LoadFromMemoryRGBA32(int w, int h, const u8* pixels)
{
    if (m_id == 0)
        Create();

    glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    m_width = w;
    m_height = h;
}

void Texture::LoadFromFile(const std::filesystem::path& path)
{
    SDL::Surface s{path};
    s.ConvertFormat(SDL_PIXELFORMAT_RGBA32);

    return LoadFromMemoryRGBA32(s.GetWidth(), s.GetHeight(), static_cast<u8*>(s.GetPixels()));
}

void Texture::SetFilter(TextureFilter filter)
{
    int v = filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, v);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, v);
}

void Texture::UpdateSize(s32 width, s32 height)
{
    m_width = width;
    m_height = height;
}

}

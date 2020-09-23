#include "sdl/error.h"
#include "sdl/font.h"
#include "sdl/surface.h"

#include <fmt/format.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

namespace SDL {

Surface::Surface() :
	m_ptr{nullptr, SDL_FreeSurface}
{}

Surface::Surface(const std::filesystem::path& path) :
    Surface{}
{
    m_ptr.reset(IMG_Load(path.string().c_str()));

    if (!m_ptr)
        throw Error{"IMG_Load"};
}

Surface::Surface(const u8* data, std::size_t size) :
    Surface{}
{
    m_ptr.reset(IMG_Load_RW(SDL_RWFromConstMem(data, static_cast<int>(size)), true));

    if (!m_ptr)
        throw Error{"IMG_Load_RW"};
}

Surface::Surface(const Font& font, const std::string& text, const SDL_Color& color) :
    Surface{}
{
    m_ptr.reset(TTF_RenderUTF8_Blended(font.Get(), text.c_str(), color));

    if (!m_ptr)
        throw Error{"TTF_RenderUTF8_Blended"};
}

Surface::Surface(const Font& font, u16 ch, const SDL_Color& color) :
    Surface{}
{
    m_ptr.reset(TTF_RenderGlyph_Blended(font.Get(), ch, color));

    if (!m_ptr) {
        if (font.GlyphIsProvided(ch)) {
            Logger::Log(LogCategory::Warn, "SDL2", fmt::format("TTF_RenderGlyph_Blended: {} (family='{}' ch=0x{:x})", SDL_GetError(), font.GetFamilyName(), ch));
            *this = Surface{0, 0, 32, SDL_PIXELFORMAT_RGBA8888};
        }
        else {
            throw Error{"TTF_RenderGlyph_Blended"};
        }
    }
}

Surface::Surface(void* pixels, int width, int height, int depth, int pitch, SDL_PixelFormatEnum format) :
    Surface{}
{
    m_ptr.reset(SDL_CreateRGBSurfaceWithFormatFrom(pixels, width, height, depth, pitch, format));

    if (!m_ptr)
        throw Error{"SDL_CreateRGBSurfaceWithFormatFrom"};
}

Surface::Surface(int width, int height, int depth, SDL_PixelFormatEnum format) :
    Surface{}
{
    m_ptr.reset(SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, format));

    if (!m_ptr)
        throw Error{"SDL_CreateRGBSurfaceWithFormat"};
}

SDL_Surface* Surface::Get() const
{
    return m_ptr.get();
}

void Surface::Blit(Surface& src, Surface& dst, SDL_Rect& dst_rect)
{
    SDL_BlitSurface(src.Get(), nullptr, dst.Get(), &dst_rect);
}

void Surface::SetBlendMode(SDL_BlendMode mode)
{
    SDL_SetSurfaceBlendMode(Get(), mode);
}

int Surface::GetWidth()
{
    return Get()->w;
}

int Surface::GetHeight()
{
    return Get()->h;
}

void* Surface::GetPixels()
{
    return Get()->pixels;
}

void Surface::ConvertFormat(SDL_PixelFormatEnum format)
{
    m_ptr.reset(SDL_ConvertSurfaceFormat(Get(), format, 0));

    if (!m_ptr)
        throw Error{"SDL_ConvertSurfaceFormat"};
}

void Surface::SavePNG(const std::filesystem::path& path)
{
    if (IMG_SavePNG(Get(), path.string().c_str()) != 0)
        throw Error{"IMG_SavePNG"};
}

}

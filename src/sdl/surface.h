#pragma once

#include "common/types.h"

#include <filesystem>
#include <memory>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_blendmode.h>

struct SDL_Surface;
struct SDL_Color;
struct SDL_Rect;

namespace SDL {

class Font;

class Surface {
public:
    Surface();
    Surface(const std::filesystem::path& path);
    Surface(const u8* data, const std::size_t size);
    Surface(const Font& font, const std::string& text, const SDL_Color& color);
    Surface(const Font& font, u16 ch, const SDL_Color& color);
    Surface(void* pixels, int width, int height, int depth, int pitch, SDL_PixelFormatEnum format);
    Surface(int width, int height, int depth, SDL_PixelFormatEnum format);

	SDL_Surface* Get() const;
    static void Blit(Surface& src, Surface& dst, SDL_Rect& dst_rect);
    void SetBlendMode(SDL_BlendMode mode);
    int GetWidth();
    int GetHeight();
    void* GetPixels();
    void ConvertFormat(SDL_PixelFormatEnum format);
    void SavePNG(const std::filesystem::path& path);

private:
    std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> m_ptr;
};

}

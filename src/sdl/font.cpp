#include "sdl/error.h"
#include "sdl/font.h"

#include <SDL2/SDL_ttf.h>

namespace SDL {

Font::Font() :
	m_ptr{nullptr, TTF_CloseFont}
{}

Font::Font(const std::filesystem::path& path, int ptsize) :
    Font{}
{
    m_ptr.reset(TTF_OpenFont(path.string().c_str(), ptsize));

	if (!m_ptr)
		throw Error{"TTF_OpenFont"};
}

Font::Font(const u8* data, std::size_t size, int ptsize) :
    Font{}
{
    m_ptr.reset(TTF_OpenFontRW(SDL_RWFromConstMem(data, static_cast<int>(size)), true, ptsize));

    if (!m_ptr)
        throw Error{"TTF_OpenFontRW"};
}

TTF_Font* Font::Get() const
{
    return m_ptr.get();
}

void Font::SetOutline(int outline)
{
    TTF_SetFontOutline(Get(), outline);
}

bool Font::GlyphIsProvided(u16 ch) const
{
    return TTF_GlyphIsProvided(Get(), ch);
}

int Font::GlyphGetAdvance(u16 ch) const
{
    int adv;
    TTF_GlyphMetrics(Get(), ch, nullptr, nullptr, nullptr, nullptr, &adv);

    return adv;
}

int Font::GetLineSkip() const
{
    return TTF_FontLineSkip(Get());
}

std::string Font::GetFamilyName() const
{
    return TTF_FontFaceFamilyName(Get());
}

}

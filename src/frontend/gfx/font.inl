#include "frontend/gfx/font.h"

namespace Gfx {

inline const Font::Glyph& Font::GetGlyph(u16 ch)
{
	return m_glyphs[ch];
}

inline u32 Font::GetFillTexture(u32 index)
{
	return m_fill_textures[index];
}

inline u32 Font::GetOutlineTexture(u32 index)
{
	return m_outline_textures[index];
}

inline s16 Font::GetTextureSize() const
{
	return m_texture_size;
}

inline s32 Font::GetOutlineThickness() const {
	return m_outline_thickness;
}

inline s32 Font::GetLineSkip() const {
	return m_fill_font.GetLineSkip();
}

inline bool Font::IsValid() const
{
    return m_valid;
}

}

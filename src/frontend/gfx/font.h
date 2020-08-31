#pragma once

#include "common/types.h"
#include "frontend/gfx/texture.h"
#include "imgui/imgui.h"
#include "sdl/font.h"

#include <array>
#include <filesystem>
#include <unordered_map>
#include <vector>

#include <stb_rect_pack.h>

namespace Gfx {

class Font {
public:
    struct Glyph {
		s16 x, y, w, h, adv;
		u8 page;
	};

	using Glyphs = std::unordered_map<u16, Glyph>;

    void LoadFromFile(const std::filesystem::path& path, int ptsize, int outline);
    void LoadFromMemory(const u8* data, std::size_t size, int ptsize, int outline);
    void CacheGlyphRange(u16 min, u16 max);
    std::vector<u32> CollectTextures();

    const Glyph& GetGlyph(u16 ch);
	u32 GetFillTexture(u32 index);
	u32 GetOutlineTexture(u32 index);
	s16 GetTextureSize() const;
	s32 GetOutlineThickness() const;
	s32 GetLineSkip() const;
    bool IsValid() const;

    Texture RenderText(const std::string& str, const ImColor& fgcol, const ImColor& bgcol, TextureFilter filter);

private:
    std::array<u32, 2> GenTextures();
    void Clear();

    int m_texture_size{1024};
    int m_outline_thickness{};
	SDL::Font m_fill_font;
    SDL::Font m_outline_font;
	std::vector<u32> m_fill_textures;
	std::vector<u32> m_outline_textures;
	stbrp_context m_pack_ctx{};
	std::vector<stbrp_node> m_nodes{};
    Glyphs m_glyphs;
    bool m_valid{};
};

}

#include "frontend/gfx/font.inl"

#include "common/file_util.h"
#include "frontend/gfx/font.h"
#include "sdl/surface.h"

#include <GL/glew.h>
#include <SDL2/SDL_rect.h>
#include <utf8.h>

namespace Gfx {

void Font::LoadFromFile(const std::filesystem::path& path, int ptsize, int outline)
{
    Clear();

    auto font_data = FileUtil::ReadAllBytes(path);
    m_fill_font = {font_data.data(), font_data.size(), ptsize};
    m_outline_font = {font_data.data(), font_data.size(), ptsize};
    m_outline_font.SetOutline(m_outline_thickness = outline);
    CacheGlyphRange(0x0020, 0x00ff);

    m_valid = true;
}

void Font::LoadFromMemory(const u8* data, std::size_t size, int ptsize, int outline)
{
    Clear();

    m_fill_font = {data, size, ptsize};
	m_outline_font = {data, size, ptsize};
	m_outline_font.SetOutline(m_outline_thickness = outline);
    CacheGlyphRange(0x0020, 0x00ff);

    m_valid = true;
}

std::array<u32, 2> Font::GenTextures()
{
	std::array<u32, 2> ids;
	glGenTextures(2, ids.data());

	for (auto id : ids) {
		glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_texture_size, m_texture_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}

    return ids;
}

void Font::CacheGlyphRange(u16 min, u16 max)
{
	if (m_glyphs.empty()) {
		m_nodes.resize(m_texture_size);
		stbrp_init_target(&m_pack_ctx, m_texture_size, m_texture_size, m_nodes.data(), m_texture_size);

		auto ids = GenTextures();
		m_fill_textures.push_back(ids[0]);
		m_outline_textures.push_back(ids[1]);
	}

	std::vector<u16> to_cache;
    to_cache.reserve(max - min);
    std::unordered_map<u16, std::array<SDL::Surface, 2>> surfaces;
    std::vector<stbrp_rect> rects, not_packed;
    rects.reserve(max - min);
    bool ok{};

    for (u16 i = min; i <= max; ++i) {
        if (m_fill_font.GlyphIsProvided(i))
            to_cache.push_back(i);
    }

	for (u16 ch : to_cache) {
		auto s1 = SDL::Surface{m_fill_font, ch, {255, 255, 255, 255}};
		auto s2 = SDL::Surface{m_outline_font, ch, {255, 255, 255, 255}};
        auto s3 = SDL::Surface{s2.GetWidth(), s2.GetHeight(), 32, SDL_PIXELFORMAT_RGBA32};

		SDL_Rect dst{m_outline_thickness, m_outline_thickness, s2.GetWidth(), s2.GetHeight()};
		SDL::Surface::Blit(s1, s3, dst);

		m_glyphs[ch].adv = m_outline_font.GlyphGetAdvance(ch);
		rects.push_back({ch, static_cast<stbrp_coord>(s3.GetWidth()), static_cast<stbrp_coord>(s3.GetHeight()), 0, 0, 0});

        surfaces[ch] = {std::move(s3), std::move(s2)};
	}

	while (!ok) {
		ok = stbrp_pack_rects(&m_pack_ctx, rects.data(), static_cast<int>(rects.size()));

		for (auto& r : rects) {
			if (r.id == 0)
				continue;

			if (r.was_packed) {
				auto& s = surfaces[r.id];

				glBindTexture(GL_TEXTURE_2D, m_fill_textures.back());
				glTexSubImage2D(GL_TEXTURE_2D, 0, r.x, r.y, r.w, r.h, GL_RGBA, GL_UNSIGNED_BYTE, s[0].GetPixels());

				glBindTexture(GL_TEXTURE_2D, m_outline_textures.back());
				glTexSubImage2D(GL_TEXTURE_2D, 0, r.x, r.y, r.w, r.h, GL_RGBA, GL_UNSIGNED_BYTE, s[1].GetPixels());

				auto& g = m_glyphs[r.id];
				g.x = r.x;
				g.y = r.y;
				g.w = r.w;
				g.h = r.h;
				g.page = static_cast<u8>(m_fill_textures.size() - 1);

				r.id = 0;
				surfaces.erase(r.id);
			}
			else {
				not_packed.push_back(r);
            }
		}

		if (!ok) {
			stbrp_init_target(&m_pack_ctx, m_texture_size, m_texture_size, m_nodes.data(), m_texture_size);

			auto ids = GenTextures();
			m_fill_textures.push_back(ids[0]);
			m_outline_textures.push_back(ids[1]);

			rects.swap(not_packed);
            not_packed.clear();
		}
	}
}

void Font::Clear()
{
    m_fill_textures.clear();
    m_outline_textures.clear();
    m_nodes.clear();
    m_glyphs.clear();
    m_valid = false;
}

std::vector<u32> Font::CollectTextures()
{
    std::vector<u32> v;

    std::copy(m_fill_textures.begin(), m_fill_textures.end(), std::back_inserter(v));
    std::copy(m_outline_textures.begin(), m_outline_textures.end(), std::back_inserter(v));

    return v;
}

inline SDL_Color ToSDLColor(const ImColor& color)
{
    return {static_cast<Uint8>(color.Value.x * 255), static_cast<Uint8>(color.Value.y * 255),
        static_cast<Uint8>(color.Value.z * 255), static_cast<Uint8>(color.Value.w * 255)};
}

Texture Font::RenderText(const std::string& str, const ImColor& fgcol, const ImColor& bgcol, TextureFilter filter)
{
    SDL::Surface bg_surface{m_outline_font, str, ToSDLColor(bgcol)};
    SDL::Surface fg_surface{m_fill_font, str, ToSDLColor(fgcol)};
    fg_surface.SetBlendMode(SDL_BLENDMODE_BLEND);

    SDL_Rect dst_rect{m_outline_thickness, m_outline_thickness, fg_surface.GetWidth(), fg_surface.GetHeight()};
    SDL::Surface::Blit(fg_surface, bg_surface, dst_rect);

    SDL::Surface result_surface{bg_surface.GetWidth(), bg_surface.GetHeight(), 32, SDL_PIXELFORMAT_RGBA32};
    dst_rect = {0, 0, bg_surface.GetWidth(), bg_surface.GetHeight()};
    SDL::Surface::Blit(bg_surface, result_surface, dst_rect);

    Texture tex;
    tex.LoadFromMemoryRGBA32(result_surface.GetWidth(), result_surface.GetHeight(),
        static_cast<u8*>(result_surface.GetPixels()));
    tex.SetFilter(filter);

    return tex;
}

}

#include "frontend/gfx/gfx.h"

#include <array>

#include <SDL2/SDL_rect.h>
#include <utf8.h>

namespace Gfx {

std::array<ImVec2, 4> BuildUVs(int tw, int th, const ImVec4& src, FlipFlags flip) {
    std::array<ImVec2, 4> uv;

    uv[0] = {src.x / tw, src.y / th};
    uv[1] = {(src.x + src.z) / tw, src.y / th};
    uv[2] = {(src.x + src.z) / tw, (src.y + src.w) / th};
    uv[3] = {src.x / tw, (src.y + src.w) / th};

    if ((flip & FlipFlags_Horizontal) == FlipFlags_Horizontal) {
        std::swap(uv[0], uv[1]);
        std::swap(uv[2], uv[3]);
    }

    if ((flip & FlipFlags_Vertical) == FlipFlags_Vertical) {
        std::swap(uv[0], uv[3]);
        std::swap(uv[1], uv[2]);
    }

    return uv;
}

void AddSprite(ImDrawList* dl, const ImVec2& dl_origin, Texture& tex, const ImVec4& src, const ImVec4& dst,
    const ImColor& tint, FlipFlags flip)
{
    if (!tex.IsValid() || tint.Value.w == 0.0f) {
        return;
    }

    float dstx = dl_origin.x + dst.x;
    float dsty = dl_origin.y + dst.y;
    auto uv = BuildUVs(tex.GetWidth(), tex.GetHeight(), src, flip);

    dl->AddImageQuad((ImTextureID)(uintptr_t)tex.GetId(),
        {dstx, dsty}, {dstx + dst.z, dsty},
        {dstx + dst.z, dsty + dst.w}, {dstx, dsty + dst.w},
        uv[0], uv[1], uv[2], uv[3],
        ImGui::GetColorU32(tint.Value));
}

inline ImVec2 RotateVec2(const ImVec2& v, float cos_a, float sin_a)
{
    return {v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a};
}

void AddSpriteRotated(ImDrawList* dl, const ImVec2& dl_origin, Texture& tex, const ImVec4& src, const ImVec4& dst,
    const ImColor& tint, FlipFlags flip, float angle)
{
    if (!tex.IsValid() || tint.Value.w == 0.0f) {
        return;
    }

    auto off = dl_origin + ImVec2(dst.x, dst.y) + ImVec2(dst.z / 2, dst.w / 2);
    auto rcos = static_cast<float>(std::cos(angle * 0.0174533));
    auto rsin = static_cast<float>(std::sin(angle * 0.0174533));
    auto uv = BuildUVs(tex.GetWidth(), tex.GetHeight(), src, flip);

    ImVec2 vert[4] {
        off + RotateVec2({-dst.z * 0.5f, -dst.w * 0.5f}, rcos, rsin),
        off + RotateVec2({dst.z * 0.5f, -dst.w * 0.5f}, rcos, rsin),
        off + RotateVec2({dst.z * 0.5f, dst.w * 0.5f}, rcos, rsin),
        off + RotateVec2({-dst.z * 0.5f, dst.w * 0.5f}, rcos, rsin)
    };

    dl->AddImageQuad((ImTextureID)(uintptr_t)tex.GetId(),
        vert[0], vert[1], vert[2], vert[3],
        uv[0], uv[1], uv[2], uv[3],
        ImGui::GetColorU32(tint.Value));
}

inline SDL_Point ToPoint(const ImVec2& v)
{
    return {static_cast<int>(v.x), static_cast<int>(v.y)};
}

ImVec4 CalcSpriteRotatedBounds(const ImVec4& dst, float angle)
{
    ImVec2 off = ImVec2(dst.x, dst.y) + ImVec2(dst.z / 2, dst.w / 2);
    auto rcos = static_cast<float>(std::cos(angle * 0.0174533));
    auto rsin = static_cast<float>(std::sin(angle * 0.0174533));

    SDL_Point vert[4] {
        ToPoint(off + RotateVec2({-dst.z * 0.5f, -dst.w * 0.5f}, rcos, rsin)),
        ToPoint(off + RotateVec2({dst.z * 0.5f, -dst.w * 0.5f}, rcos, rsin)),
        ToPoint(off + RotateVec2({dst.z * 0.5f, dst.w * 0.5f}, rcos, rsin)),
        ToPoint(off + RotateVec2({-dst.z * 0.5f, dst.w * 0.5f}, rcos, rsin))
    };

    SDL_Rect r;
    SDL_EnclosePoints(vert, 4, nullptr, &r);

    return {static_cast<float>(r.x), static_cast<float>(r.y), static_cast<float>(r.w), static_cast<float>(r.h)};
}

void AddText(ImDrawList* dl, const ImVec2& dl_origin, Font& font, const char* str_beg, const char* str_end,
    const ImVec2& pos, const ImColor& fgcol, const ImColor& bgcol, const ImVec2& scale)
{
    if (!font.IsValid() || (fgcol.Value.w == 0.0f && bgcol.Value.w == 0.0f)) {
        return;
    }

	auto tex_size = static_cast<float>(font.GetTextureSize());
    std::vector<std::pair<u16, const Font::Glyph*>> gc;
    gc.reserve(str_end - str_beg);

    while (str_beg != str_end) {
		auto ch = static_cast<u16>(utf8::next(str_beg, str_end));

        if (ch == '\n' || ch == '\t')
            gc.push_back(std::make_pair(ch, nullptr));
        else
            gc.push_back(std::make_pair(ch, &font.GetGlyph(ch)));
    }

    int j_min{}, j_max = 2;
    if (bgcol.Value.w == 0.0f) ++j_min;
    if (fgcol.Value.w == 0.0f) --j_max;

    for (int j = j_min; j < j_max; ++j) {
        auto off = pos + dl_origin;

        for (auto& i : gc) {
            auto ch = i.first;
            auto g = i.second;

            if (ch == '\n') {
                off.x = pos.x + dl_origin.x;
                off.y += std::floor(font.GetLineSkip() * scale.y);
            }
            else if (ch == '\t') {
                off.x += (2 * font.GetLineSkip()) * scale.x;
            }
            else {
                auto p_min = off;
                auto p_max = off + ImVec2{std::floor((g->w + 0.3f) * scale.x), std::floor((g->h + 0.3f) * scale.y)};
                auto uv_min = ImVec2{g->x / tex_size, g->y / tex_size};
                auto uv_max = ImVec2{(g->x + g->w) / tex_size, (g->y + g->h) / tex_size};

                if (j == 0)
                    dl->AddImage((ImTextureID)(uintptr_t)font.GetOutlineTexture(g->page), p_min, p_max, uv_min, uv_max, bgcol);
                else
                    dl->AddImage((ImTextureID)(uintptr_t)font.GetFillTexture(g->page), p_min, p_max, uv_min, uv_max, fgcol);

                off.x += std::floor((g->adv + std::floor(font.GetOutlineThickness() / 2.0f)) * scale.x);
            }
        }
    }
}

ImVec2 CalcTextSize(Font& font, const char* str_beg, const char* str_end, const ImVec2& scale)
{
    if (!font.IsValid()) {
        return {};
    }

	ImVec2 off{};
    ImVec2 bounds{};

    while (str_beg != str_end) {
        auto ch = static_cast<u16>(utf8::next(str_beg, str_end));

        if (ch == '\n') {
            off.x = 0.0f;
            off.y += std::floor(font.GetLineSkip() * scale.y);
        }
        else if (ch == '\t') {
            off.x += (2 * font.GetLineSkip()) * scale.x;
        }
        else {
            auto& g = font.GetGlyph(ch);
            auto p_max = off + ImVec2{std::floor((g.w + 0.3f) * scale.x), std::floor((g.h + 0.3f) * scale.y)};

            off.x += std::floor((g.adv + std::floor(font.GetOutlineThickness() / 2.0f)) * scale.x);

            bounds.x = std::max(bounds.x, p_max.x);
            bounds.y = std::max(bounds.y, p_max.y);
        }
    }

    bounds.x = std::ceil(bounds.x);
    bounds.y = std::ceil(bounds.y);

    return bounds;
}

}

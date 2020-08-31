#pragma once

#include "common/imvec2_ops.h"
#include "frontend/gfx/font.h"
#include "frontend/gfx/texture.h"

namespace Gfx {

enum FlipFlags {
    FlipFlags_None,
    FlipFlags_Horizontal,
    FlipFlags_Vertical
};

void AddSprite(ImDrawList* dl, const ImVec2& dl_origin, Texture& tex, const ImVec4& src, const ImVec4& dst,
    const ImColor& tint, FlipFlags flip);
void AddSpriteRotated(ImDrawList* dl, const ImVec2& dl_origin, Texture& tex, const ImVec4& src, const ImVec4& dst,
    const ImColor& tint, FlipFlags flip, float angle);
ImVec4 CalcSpriteRotatedBounds(const ImVec4& dst, float angle);
void AddText(ImDrawList* dl, const ImVec2& dl_origin, Font& font, const char* str_beg, const char* str_end,
    const ImVec2& pos, const ImColor& fgcol, const ImColor& bgcol, const ImVec2& scale);
ImVec2 CalcTextSize(Font& font, const char* str_beg, const char* str_end, const ImVec2& scale);

}

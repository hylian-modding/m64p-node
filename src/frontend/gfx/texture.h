#pragma once

#include "common/types.h"

#include <filesystem>

namespace Gfx {

enum class TextureFilter {
    Linear,
    Nearest
};

class Texture {
public:
    Texture() = default;
    Texture(u32 id, s32 width, s32 height);

    static u32 GetCurrentBound();
    static void Destroy(u32 id);
    void LoadFromMemoryRGBA32(int w, int h, const u8* pixels);
    void LoadFromFile(const std::filesystem::path& path);
    void SetFilter(TextureFilter filter);
    u32 GetId() const;
    s32 GetWidth() const;
    s32 GetHeight() const;
    void UpdateSize(s32 width, s32 height);
    bool IsValid() const;

private:
    u32 m_id{};
    s32 m_width{};
    s32 m_height{};

    void Create();
};

}

#include "frontend/gfx/texture.inl"

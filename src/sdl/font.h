#pragma once

#include "common/types.h"

#include <filesystem>
#include <memory>

typedef struct _TTF_Font TTF_Font;

namespace SDL {

class Font {
public:
    Font();
    Font(const std::filesystem::path& path, int ptsize);
    Font(const u8* data, std::size_t size, int ptsize);

	TTF_Font* Get() const;
    void SetOutline(int outline);
    bool GlyphIsProvided(u16 ch) const;
    int GlyphGetAdvance(u16 ch) const;
    int GetLineSkip() const;
    std::string GetFamilyName() const;

private:
    std::unique_ptr<TTF_Font, void(*)(TTF_Font*)> m_ptr;
};

}

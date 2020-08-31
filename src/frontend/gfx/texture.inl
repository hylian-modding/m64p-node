#include "frontend/gfx/texture.h"

namespace Gfx {

inline u32 Texture::GetId() const
{
    return m_id;
}

inline s32 Texture::GetWidth() const
{
    return m_width;
}

inline s32 Texture::GetHeight() const
{
    return m_height;
}

inline bool Texture::IsValid() const
{
    return m_id != 0;
}

}

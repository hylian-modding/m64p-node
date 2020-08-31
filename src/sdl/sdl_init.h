#pragma once

#include "common/types.h"

namespace SDL {

class SDLInit {
public:
    SDLInit() = default;
    ~SDLInit();
    SDLInit(const SDLInit& other) = delete;
    SDLInit& operator=(const SDLInit& other) = delete;
    SDLInit(SDLInit&& other) = delete;
    SDLInit& operator=(SDLInit&& other) = delete;

    void Init(u32 flags);
    void Quit();

private:
    bool m_init{};
};

}

#pragma once

#include "common/types.h"

namespace SDL {

class TTFInit {
public:
    TTFInit() = default;
    ~TTFInit();
    TTFInit(const TTFInit& other) = delete;
    TTFInit& operator=(const TTFInit& other) = delete;
    TTFInit(TTFInit&& other) = delete;
    TTFInit& operator=(TTFInit&& other) = delete;

    void Init();
    void Quit();

private:
    bool m_init{};
};

}

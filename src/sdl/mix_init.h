#pragma once

#include "common/types.h"

namespace SDL {

class MixInit {
public:
    MixInit() = default;
    ~MixInit();
    MixInit(const MixInit& other) = delete;
    MixInit& operator=(const MixInit& other) = delete;
    MixInit(MixInit&& other) = delete;
    MixInit& operator=(MixInit&& other) = delete;

    void Init(int flags);
    void Quit();

private:
    bool m_init{};
};

}

#pragma once

#include "common/types.h"

namespace SDL {

class IMGInit {
public:
    IMGInit() = default;
    ~IMGInit();
    IMGInit(const IMGInit& other) = delete;
    IMGInit& operator=(const IMGInit& other) = delete;
    IMGInit(IMGInit&& other) = delete;
    IMGInit& operator=(IMGInit&& other) = delete;

    void Init(int flags);
    void Quit();

private:
    bool m_init{};
};

}

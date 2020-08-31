#pragma once

#include "common/types.h"

namespace SDL {

class MixDevice {
public:
    MixDevice() = default;
    ~MixDevice();
    MixDevice(const MixDevice& other) = delete;
    MixDevice& operator=(const MixDevice& other) = delete;
    MixDevice(MixDevice&& other) = delete;
    MixDevice& operator=(MixDevice&& other) = delete;

    void Open(int frequency, u16 format, int channels, int chunk_size);
    void Close();

private:
    bool m_init{};
};

}

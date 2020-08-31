#pragma once

#include "common/types.h"
#include "m64p/fwd.h"

#include <array>

#include <m64p_api/m64p_plugin.h>

namespace Frontend {

class Input {
public:
    void Update(M64P::Core& core);

    f32 GetAxis(u32 cont, u32 axis);
    void SetAxis(u32 cont, u32 axis, f32 value);
    void SetAxisVi(u32 cont, u32 axis, f32 value);
    bool GetButton(u32 cont, u32 button);
    bool GetButtonDown(u32 cont, u32 button);
    bool GetButtonUp(u32 cont, u32 button);
    void SetButton(u32 cont, u32 button, bool pressed);
    void SetButtonDown(u32 cont, u32 button);

private:
    static constexpr unsigned NumCont = 4;
    static constexpr u32 NumButtons = 14;
    static constexpr u32 NumAxis = 2;

    using ContState = std::array<BUTTONS, NumCont>;

    ContState m_curr{};
    ContState m_prev{};
    ContState m_btn_down{};
    ContState m_btn_up{};
    ContState m_persist{};
    ContState m_vi{};
};

}

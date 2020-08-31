#include "frontend/input.h"
#include "m64p/core.h"

#include <algorithm>

namespace Frontend {

inline bool GetButtonState(BUTTONS& state, u32 button)
{
    return (state.Value & (1u << button)) == (1u << button);
}

inline void SetButtonState(BUTTONS& state, u32 button, bool pressed)
{
    if (pressed)
        state.Value |= (1u << button);
    else
        state.Value &= ~(1u << button);
}

void Input::Update(M64P::Core& core)
{
    for (u32 i{}; i < NumCont; ++i)
        core.ContSetInput(i, m_persist[i].Value | m_vi[i].Value);

    for (u32 i{}; i < NumCont; ++i) {
        auto& curr = m_curr[i];
        auto& prev = m_prev[i];

        prev.Value = curr.Value;
        curr.Value = core.ContGetInput(i);

        for (u32 j{}; j < NumButtons; ++j) {
            SetButtonState(m_btn_down[i], j, GetButtonState(curr, j) && !GetButtonState(prev, j));
            SetButtonState(m_btn_up[i], j, !GetButtonState(curr, j) && GetButtonState(prev, j));
        }

        m_vi.fill({});
    }
}

f32 Input::GetAxis(u32 cont, u32 axis)
{
    switch (axis) {
    case 0:
        return std::clamp(m_curr[cont].X_AXIS / 80.0f, -1.0f, 1.0f);
    case 1:
        return std::clamp(m_curr[cont].Y_AXIS / 80.0f, -1.0f, 1.0f);
    default:
        return 0;
    }
}

void Input::SetAxis(u32 cont, u32 axis, f32 value)
{
    const auto v = std::clamp(static_cast<s32>(value * 80), -80, 80);

    switch (axis) {
    case 0:
        m_persist[cont].X_AXIS = v;
        break;
    case 1:
        m_persist[cont].Y_AXIS = v;
    }
}

void Input::SetAxisVi(u32 cont, u32 axis, f32 value)
{
    const auto v = std::clamp(static_cast<s32>(value * 80), -80, 80);

    switch (axis) {
    case 0:
        m_vi[cont].X_AXIS = v;
        break;
    case 1:
        m_vi[cont].Y_AXIS = v;
    }
}

bool Input::GetButton(u32 cont, u32 button)
{
    return GetButtonState(m_curr[cont], button);
}

bool Input::GetButtonDown(u32 cont, u32 button)
{
    return GetButtonState(m_btn_down[cont], button);
}

bool Input::GetButtonUp(u32 cont, u32 button)
{
    return GetButtonState(m_btn_up[cont], button);
}

void Input::SetButton(u32 cont, u32 button, bool pressed)
{
    SetButtonState(m_persist[cont], button, pressed);
}

void Input::SetButtonDown(u32 cont, u32 button)
{
    SetButtonState(m_vi[cont], button, true);
}

}

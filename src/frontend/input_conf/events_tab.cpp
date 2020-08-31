#include "common/imgui_util.h"
#include "frontend/input_conf/events_tab.h"
#include "imgui/imgui.h"
#include "m64p/core.h"

#include <cassert>

#include <fmt/format.h>
#include <SDL2/SDL_events.h>

namespace InputConf {

using namespace MapUtil;

const std::array<const char*, EventIndex_NumEvents> k_labels {
    "Stop", "FullScreen", "Save State", "Load State", "Increment Slot", "Reset",
    "Speed Down", "Speed Up", "Screenshot", "Pause", "Mute", "Increase Volume",
    "Decrease Volume", "Fast Forward", "Frame Advance", "GameShark"
};

const std::array<const char*, EventIndex_NumEvents> k_conf_labels {
    "Kbd Mapping Stop", "Kbd Mapping Fullscreen", "Kbd Mapping Save State",
    "Kbd Mapping Load State", "Kbd Mapping Increment Slot", "Kbd Mapping Reset",
    "Kbd Mapping Speed Down", "Kbd Mapping Speed Up", "Kbd Mapping Screenshot",
    "Kbd Mapping Pause", "Kbd Mapping Mute", "Kbd Mapping Increase Volume",
    "Kbd Mapping Decrease Volume", "Kbd Mapping Fast Forward", "Kbd Mapping Frame Advance",
    "Kbd Mapping GameShark"
};

const std::array<unsigned, EventIndex_NumEvents> s_conf_defaults {
    27, 0, 286, 288, 0, 290, 291, 292, 293, 112, 109, 93, 91, 102, 47, 103
};

EventsTab::EventsTab()
{
    m_kbd_map_str.fill("None");
}

void EventsTab::LoadConfig(M64P::Core& core)
{
    auto section = core.ConfigOpenSection("CoreEvents");

    for (int i{}; i < EventIndex_NumEvents; ++i)
        m_kbd_map[i] = ParseKbdEvent(std::to_string(section.GetIntOr(k_conf_labels[i], s_conf_defaults[i])));

    for (int i = 0; i < EventIndex_NumEvents; ++i)
        m_kbd_map_str[i] = FormatKbdEvent(m_kbd_map[i], true);
}

void EventsTab::SaveConfig(M64P::Core& core)
{
    auto section = core.ConfigOpenSection("CoreEvents");

    for (int i{}; i < EventIndex_NumEvents; ++i)
        section.SetInt(k_conf_labels[i], GetKbdEventKey(m_kbd_map[i]));

    core.ConfigSaveFile();
}

void EventsTab::DoEvent(const SDL_Event& e)
{
    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
        m_map_index = EventIndex_None;

    if (m_map_index > EventIndex_None && e.type == SDL_KEYDOWN) {
        MapEvent m{};
        bool ignore{};

        if (e.key.keysym.scancode != SDL_SCANCODE_ESCAPE) {
            m.type = MapType::Key;
            m.key.scancode = e.key.keysym.scancode;
            m.key.mod = e.key.keysym.mod;

            if (((m.key.mod & KMOD_LCTRL) == KMOD_LCTRL || (m.key.mod & KMOD_RCTRL) == KMOD_RCTRL)
            && ((m.key.mod & KMOD_LSHIFT) == KMOD_LSHIFT || (m.key.mod & KMOD_RSHIFT) == KMOD_RSHIFT))
                m.key.mod = KMOD_CTRL | KMOD_SHIFT;
            else if (((m.key.mod & KMOD_LCTRL) == KMOD_LCTRL || (m.key.mod & KMOD_RCTRL) == KMOD_RCTRL)
            && ((m.key.mod & KMOD_LALT) == KMOD_LALT || (m.key.mod & KMOD_RALT) == KMOD_RALT))
                 m.key.mod = KMOD_CTRL | KMOD_ALT;
            else if ((m.key.mod & KMOD_LCTRL) == KMOD_LCTRL || (m.key.mod & KMOD_RCTRL) == KMOD_RCTRL)
                 m.key.mod = KMOD_CTRL;
            else if (((m.key.mod & KMOD_LSHIFT) == KMOD_LSHIFT || (m.key.mod & KMOD_RSHIFT) == KMOD_RSHIFT)
            && ((m.key.mod & KMOD_LALT) == KMOD_LALT || (m.key.mod & KMOD_RALT) == KMOD_RALT))
                 m.key.mod = KMOD_SHIFT | KMOD_ALT;
            else if ((m.key.mod & KMOD_LSHIFT) == KMOD_LSHIFT || (m.key.mod & KMOD_RSHIFT) == KMOD_RSHIFT)
                m.key.mod = KMOD_SHIFT;
            else if ((m.key.mod & KMOD_LALT) == KMOD_LALT || (m.key.mod & KMOD_RALT) == KMOD_RALT)
                m.key.mod = KMOD_ALT;
            else
                m.key.mod = KMOD_NONE;

            if ((m.key.scancode == SDL_SCANCODE_LCTRL && (m.key.mod & KMOD_LCTRL) == KMOD_LCTRL)
            || (m.key.scancode == SDL_SCANCODE_RCTRL && (m.key.mod & KMOD_RCTRL) == KMOD_LCTRL)
            || (m.key.scancode == SDL_SCANCODE_LSHIFT && (m.key.mod & KMOD_LSHIFT) == KMOD_LSHIFT)
            || (m.key.scancode == SDL_SCANCODE_RSHIFT && (m.key.mod & KMOD_RSHIFT) == KMOD_RSHIFT)
            || (m.key.scancode == SDL_SCANCODE_LALT && (m.key.mod & KMOD_LALT) == KMOD_LALT)
            || (m.key.scancode == SDL_SCANCODE_RALT && (m.key.mod & KMOD_RALT) == KMOD_RALT))
                ignore = true;
        }
        else
            m.type = MapType::None;

        if (!ignore) {
            m_kbd_map[m_map_index] = m;
            m_kbd_map_str[m_map_index] = FormatKbdEvent(m, true);

            AdvanceMapIndex();
        }
    }
}

void EventsTab::Show()
{
    EventIndex next = EventIndex_None;
    ShowMapButtons(&next, EventIndex_KbdStop, EventIndex_KbdGameShark);

    if (next != EventIndex_None)
        m_map_index = next;
    else if (next == EventIndex_None && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        m_map_index = EventIndex_None;
}

void EventsTab::ShowMapButtons(EventIndex* next, EventIndex min, EventIndex max)
{
    assert(next);

    for (int i = min, j = 0; i <= max; ++i, ++j) {
        ImGui::PushID(i);

        ImGui::AlignTextToFramePadding();
        ImGuiUtil::LeftLabel(k_labels[j], 120.0f);

        if (i == m_map_index) {
            auto col = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
            ImGui::PushStyleColor(ImGuiCol_Button, col);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {col.x + 0.18f, col.y + 0.18f, col.z + 0.18f, col.w - 0.22f});
        }

        if (ImGui::Button(m_kbd_map_str[i].c_str(), {200.0f, 0.0f}))
            *next = static_cast<EventIndex>(i);

        if (i == m_map_index) {
            ImGui::SetScrollHereY();
            ImGui::PopStyleColor(2);
        }

        ImGui::PopID();
    }
}

void EventsTab::AdvanceMapIndex()
{
    const auto map = [this](EventIndex min, EventIndex max) {
        if (m_map_index >= min && m_map_index <= max) {
            m_map_index = static_cast<EventIndex>(m_map_index + 1);

            if (m_map_index > max)
                m_map_index = EventIndex_None;
        }
    };

    map(EventIndex_KbdStop, EventIndex_KbdGameShark);
}

}

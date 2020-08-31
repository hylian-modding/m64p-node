#include "common/imgui_util.h"
#include "common/string_util.h"
#include "frontend/input_conf/cont_tab.h"
#include "imgui/imgui.h"
#include "m64p/core.h"

#include <cassert>
#include <algorithm>
#include <chrono>
#include <thread>
#include <tuple>

#include <fmt/format.h>
#include <SDL2/SDL_events.h>

namespace InputConf {

using namespace MapUtil;

const std::array<const char*, 6> k_plugin_names {
    "", "None", "Memory pak", "", "Transfer pak", "Rumble pak"
};

const std::array<const char*, MapIndex_Count> k_labels {
    "X Axis Left", "X Axis Right", "Y Axis Up", "Y Axis Down",
    "DPad Left", "DPad Right", "DPad Up", "DPad Down",
    "Start", "Z Trigger", "B Button", "A Button",
    "C Button Left", "C Button Right", "C Button Up", "C Button Down",
    "L Trigger", "R Trigger",
    "C Button Left", "C Button Right", "C Button Up", "C Button Down",
    "Memory Pak", "Rumble Pak"
};

const std::array<std::pair<const char*, MapIndex>, 12> k_conf_props {
    std::make_pair("DPad R", MapIndex_DPadRight),
    {"DPad L", MapIndex_DPadLeft},
    {"DPad D", MapIndex_DPadDown},
    {"DPad U", MapIndex_DPadUp},
    {"Start", MapIndex_Start},
    {"Z Trig", MapIndex_TrigZ},
    {"B Button", MapIndex_B},
    {"A Button", MapIndex_A},
    {"R Trig", MapIndex_TrigR},
    {"L Trig", MapIndex_TrigL},
    {"Mempak switch", MapIndex_MempakSwitch},
    {"Rumblepak switch", MapIndex_RumbleSwitch}
};

const std::array<std::tuple<const char*, MapIndex, MapIndex>, 4> k_conf_props_2 {
    std::make_tuple("C Button R", MapIndex_CRight, MapIndex_CRight2),
    {"C Button L", MapIndex_CLeft, MapIndex_CLeft2},
    {"C Button D", MapIndex_CDown, MapIndex_CDown2},
    {"C Button U", MapIndex_CUp, MapIndex_CUp2}
};

const int k_kbd_device_index = 0;

ContTab::ContTab(int cont_index) :
    m_cont_index{cont_index}
{
    UpdateDeviceList();
}

void ContTab::LoadConfig(M64P::Core& core)
{
    auto section = core.ConfigOpenSection(fmt::format("Input-SDL-Control{}", m_cont_index + 1).c_str());

    auto device_index = section.GetIntOr("device", 0) + 1;
    auto device_name = section.GetStringOr("name", "Keyboard");
    RememberDevice(device_index, device_name);

    m_plugged = section.GetBoolOr("plugged", false);
    m_plugin_index = section.GetIntOr("plugin", 2);

    auto ana_deadzone = section.GetStringOr("AnalogDeadzone", "4096,4096");
    int ana_deadzone_x, ana_deadzone_y;

    if (std::sscanf(ana_deadzone.c_str(), "%d,%d", &ana_deadzone_x, &ana_deadzone_y) == 2) {
        m_ana_deadzones[0] = ShortToPercent(ana_deadzone_x);
        m_ana_deadzones[1] = ShortToPercent(ana_deadzone_y);
    }
    else {
        m_ana_deadzones[0] = ShortToPercent(4096);
        m_ana_deadzones[1] = ShortToPercent(4096);
    }

    auto ana_peak = section.GetStringOr("AnalogPeak", "32768,32768");
    int ana_peak_x, ana_peak_y;

    if (std::sscanf(ana_peak.c_str(), "%d,%d", &ana_peak_x, &ana_peak_y) == 2) {
        m_ana_peaks[0] = ShortToPercent(ana_peak_x);
        m_ana_peaks[1] = ShortToPercent(ana_peak_y);
    }
    else {
        m_ana_peaks[0] = ShortToPercent(32768);
        m_ana_peaks[1] = ShortToPercent(32768);
    }

    for (const auto& prop : k_conf_props) {
        m_map[prop.second] = Parse(section.GetStringOr(prop.first, ""));
        m_map_str[prop.second] = Format(m_map[prop.second], true);
    }

    for (const auto& prop : k_conf_props_2) {
        auto str = section.GetStringOr(std::get<0>(prop), "");
        auto strs = StringUtil::Split(str, ')', true);

        if (strs.size() >= 1) {
            m_map[std::get<1>(prop)] = Parse(strs[0]);
            m_map_str[std::get<1>(prop)] = Format(m_map[std::get<1>(prop)], true);
        }

        if (strs.size() >= 2) {
            StringUtil::TrimLeftNoCopy(strs[1]);
            m_map[std::get<2>(prop)] = Parse(strs[1]);
            m_map_str[std::get<2>(prop)] = Format(m_map[std::get<2>(prop)], true);
        }
    }

    auto xaxis = ParsePair(section.GetStringOr("X Axis", ""));
    auto yaxis = ParsePair(section.GetStringOr("Y Axis", ""));

    std::array<std::tuple<MapIndex, MapEvent*>, 4> axis{
        std::make_tuple(MapIndex_XAxisLeft, &xaxis.first),
        {MapIndex_XAxisRight, &xaxis.second},
        {MapIndex_YAxisUp, &yaxis.first},
        {MapIndex_YAxisDown, &yaxis.second}
    };

    for (const auto& i : axis) {
        auto& index = std::get<0>(i);
        m_map[index] = *std::get<1>(i);
        m_map_str[index] = Format(m_map[index], true);
    }

    for (int i = 0; i < MapIndex_Count; ++i)
        m_map_str[i] = Format(m_map[i], true);
}

void ContTab::RememberDevice(int device_index, const std::string& device_name)
{
    auto same_device_count = std::count(m_devices.begin(), m_devices.end(), device_name);

    if (same_device_count == 0)
        m_device_index = k_kbd_device_index;
    else if (same_device_count == 1)
        UpdateDeviceIndex(device_name);
    else {
        if (device_index <= 0 || device_index >= static_cast<int>(m_devices.size()))
            m_device_index = k_kbd_device_index;
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds{50});
            SDL::Joystick::Update();

            if (SDL::Joystick::GetNameForIndex(device_index - 1) == device_name)
                OpenDeviceIndex(device_index);
            else
                m_device_index = k_kbd_device_index;
        }
    }
}

void ContTab::SaveConfig(M64P::Core& core)
{
    auto section = core.ConfigOpenSection(fmt::format("Input-SDL-Control{}", m_cont_index + 1).c_str());

    section.SetFloat("version", 2.0f);
    section.SetInt("mode", 0);
    section.SetInt("device", m_device_index - 1);
    section.SetString("name", m_devices[m_device_index].c_str());
    section.SetBool("plugged", m_plugged);
    section.SetInt("plugin", m_plugin_index);
    section.SetString("AnalogDeadzone", fmt::format("{},{}",
        ShortFromPercent(m_ana_deadzones[0]),
        ShortFromPercent(m_ana_deadzones[1])
        ).c_str());
    section.SetString("AnalogPeak", fmt::format("{},{}",
        ShortFromPercent(m_ana_peaks[0]),
        ShortFromPercent(m_ana_peaks[1])
        ).c_str());

    for (const auto& prop : k_conf_props)
        section.SetString(prop.first, Format(m_map[prop.second], false).c_str());

    for (const auto& prop : k_conf_props_2)
        section.SetString(std::get<0>(prop), StringUtil::Trim(fmt::format("{} {}",
            Format(m_map[std::get<1>(prop)], false),
            Format(m_map[std::get<2>(prop)], false)
            )).c_str());

    section.SetString("X Axis", FormatPair(
        { m_map[MapIndex_XAxisLeft], m_map[MapIndex_XAxisRight] }
        ).c_str());
    section.SetString("Y Axis", FormatPair(
        { m_map[MapIndex_YAxisUp], m_map[MapIndex_YAxisDown] }
        ).c_str());

    core.ConfigSaveFile();
}

void ContTab::DoEvent(const SDL_Event& e, bool focused)
{
    if (e.type == SDL_JOYDEVICEADDED) {
        if (focused) {
            UpdateDeviceList();
            OpenDeviceIndex(e.jdevice.which + 1);
        }
        else {
            UpdateDeviceListAndIndex();
        }
    }
    else if (e.type == SDL_JOYDEVICEREMOVED) {
        if (m_device_index == e.jdevice.which + 1) {
            UpdateDeviceList();
            m_device_index = k_kbd_device_index;
        }
        else {
            UpdateDeviceListAndIndex();
        }
    }
    else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
        m_map_index = MapIndex_None;

    if (focused && m_map_index > MapIndex_None) {
        if (e.type == SDL_KEYDOWN) {
            MapEvent m{};

            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                m.type = MapType::None;
            }
            else {
                m.type = MapType::Key;
                m.key.scancode = e.key.keysym.scancode;
            }

            m_map[m_map_index] = m;
            m_map_str[m_map_index] = Format(m, true);
            AdvanceMapIndex();
        }

        if (m_device_index > k_kbd_device_index) {
            if (e.type == SDL_JOYBUTTONDOWN && e.jbutton.which == m_joystick.GetId() && !m_key_map) {
                MapEvent m;
                m.type = MapType::JoyButton;
                m.jbutton.button = e.jbutton.button;

                if (!IsSame(m, m_prev_map)) {
                    m_map[m_map_index] = m;
                    m_map_str[m_map_index] = Format(m, true);

                    AdvanceMapIndex();
                    m_key_map = true;
                }
            }
            else if (e.type == SDL_JOYHATMOTION && e.jhat.which == m_joystick.GetId() && !m_key_map) {
                switch (e.jhat.value) {
                case SDL_HAT_CENTERED:
                case SDL_HAT_RIGHTUP:
                case SDL_HAT_RIGHTDOWN:
                case SDL_HAT_LEFTUP:
                case SDL_HAT_LEFTDOWN:
                    return;
                }

                MapEvent m;
                m.type = MapType::JoyHat;
                m.jhat.hat = e.jhat.hat;

                switch (e.jhat.value) {
                case SDL_HAT_UP:
                    m.jhat.dir = JoyHatDir::Up;
                    break;
                case SDL_HAT_RIGHT:
                    m.jhat.dir = JoyHatDir::Right;
                    break;
                case SDL_HAT_DOWN:
                    m.jhat.dir = JoyHatDir::Down;
                    break;
                case SDL_HAT_LEFT:
                    m.jhat.dir = JoyHatDir::Left;
                    break;
                }

                if (!IsSame(m, m_prev_map)) {
                    m_map[m_map_index] = m;
                    m_map_str[m_map_index] = Format(m, true);

                    AdvanceMapIndex();
                    m_key_map = true;
                }
            }
            else if (e.type == SDL_JOYAXISMOTION && e.jaxis.which == m_joystick.GetId() && !m_key_map)
            {
                bool in_range = e.jaxis.value < m_joy_init_axes[e.jaxis.axis] - 18000
                    || e.jaxis.value > m_joy_init_axes[e.jaxis.axis] + 18000;

                if (!in_range)
                    return;

                MapEvent m;
                m.type = MapType::JoyAxis;
                m.jaxis.axis = e.jaxis.axis;
                m.jaxis.dir = e.jaxis.value < m_joy_init_axes[e.jaxis.axis] ?
                    JoyAxisDir::Negative : JoyAxisDir::Positive;
                m.jaxis.deadzone = 0.0f;

                if (!IsSame(m, m_prev_map)) {
                    m_map[m_map_index] = m;
                    m_map_str[m_map_index] = Format(m, true);
                    m_prev_map = m;

                    AdvanceMapIndex();
                    m_key_map = true;
                }
            }
        }
    }
}

void ContTab::AdvanceMapIndex()
{
    const auto map = [this](MapIndex min, MapIndex max) {
        if (m_map_index >= min && m_map_index <= max) {
            m_map_index = static_cast<MapIndex>(m_map_index + 1);

            if (m_map_index > max)
                m_map_index = MapIndex_None;
        }
    };

    map(MapIndex_XAxisLeft, MapIndex_TrigR);
    map(MapIndex_CLeft2, MapIndex_CDown2);
    map(MapIndex_MempakSwitch, MapIndex_RumbleSwitch);
}

void ContTab::UpdateDeviceList()
{
    m_devices.clear();
    m_devices.emplace_back("Keyboard");

    for (int i{}; i < SDL::Joystick::GetCount(); ++i)
        m_devices.emplace_back(SDL::Joystick::GetNameForIndex(i));
}

void ContTab::UpdateDeviceIndex(std::string device_name)
{
    std::size_t device_index{};

    for (std::size_t i{}; i < m_devices.size(); ++i) {
        if (m_devices[i] == device_name) {
            device_index = i;
            break;
        }
    }

    if (device_index > 0)
        OpenDeviceIndex(static_cast<int>(device_index));
    else
        m_device_index = k_kbd_device_index;
}

void ContTab::UpdateDeviceListAndIndex()
{
    std::string device_name = m_devices[m_device_index];

    UpdateDeviceList();
    UpdateDeviceIndex(device_name);
}

void ContTab::OpenDeviceIndex(int device_index)
{
    int sdl_device_index = device_index - 1;
    assert(sdl_device_index > -1);

    m_joystick = SDL::Joystick{sdl_device_index};

    std::this_thread::sleep_for(std::chrono::milliseconds{50});
    SDL::Joystick::Update();

    m_joy_init_axes.resize(m_joystick.GetNumAxes());

    for (int i{}; i < m_joystick.GetNumAxes(); ++i)
        m_joy_init_axes[i] = m_joystick.GetAxis(i);

    m_device_index = device_index;
}

void ContTab::Show()
{
    m_key_map = false;

    ImGui::Checkbox("Plugged", &m_plugged);

    ShowDevicesCombo();
    ShowPluginsCombo();

    ImGui::Separator();

    if (ImGui::DragFloat2("Ana. deadzone", m_ana_deadzones.data(), 0.3f, 0.0f, 100.0f, "%.2f%%")) {
        for (auto& i : m_ana_deadzones)
            i = std::clamp(i, 0.0f, 100.0f);
    }

    if (ImGui::DragFloat2("Ana. peak", m_ana_peaks.data(), 0.3f, 0.0f, 100.0f, "%.2f%%")) {
        for (auto& i : m_ana_peaks)
            i = std::clamp(i, 0.0f, 100.0f);
    }

    ImGui::Separator();

    MapIndex next = MapIndex_None;
    ShowMapButtons(&next, MapIndex_XAxisLeft, MapIndex_TrigR, true);

    if (ImGui::CollapsingHeader("Secondary C Buttons"))
        ShowMapButtons(&next, MapIndex_CLeft2, MapIndex_CDown2);

    if (ImGui::CollapsingHeader("Insert Pak"))
        ShowMapButtons(&next, MapIndex_MempakSwitch, MapIndex_RumbleSwitch);

    if (next != MapIndex_None)
        m_map_index = next;
    else if (next == MapIndex_None && ImGui::GetIO().MouseClicked[0])
        m_map_index = MapIndex_None;

    AxisStepCheck();
}

void ContTab::ShowDevicesCombo()
{
    if (ImGui::BeginCombo("Device", fmt::format("{}: {}", m_device_index, m_devices[m_device_index]).c_str())) {
        for (std::size_t i{}; i < m_devices.size(); ++i) {
            ImGui::PushID(static_cast<int>(i));

            if (ImGui::Selectable(fmt::format("{}: {}", i, m_devices[i]).c_str())) {
                if (i == 0)
                    m_device_index = k_kbd_device_index;
                else
                    OpenDeviceIndex(static_cast<int>(i));
            }

            ImGui::PopID();
        }

        ImGui::EndCombo();
    }
}

void ContTab::ShowPluginsCombo()
{
    if (ImGui::BeginCombo("Plugin", k_plugin_names[m_plugin_index])) {
        for (std::size_t i{}; i < k_plugin_names.size(); ++i) {
            if (i == 0 || i == 3)
                continue;

            if (ImGui::Selectable(k_plugin_names[i]))
                m_plugin_index = static_cast<int>(i);
        }

        ImGui::EndCombo();
    }
}

void ContTab::ShowMapButtons(MapIndex* next, MapIndex min, MapIndex max, bool axis_mismatch_check)
{
    for (int i = min; i <= max; ++i) {
        ImGui::PushID(i);

        ImGui::AlignTextToFramePadding();
        ImGuiUtil::LeftLabel(k_labels[i], 120.0f);

        bool axis_mismatch = false;

        if (axis_mismatch_check) {
            bool wrong_xaxis = (i == MapIndex_XAxisLeft || i == MapIndex_XAxisRight)
                && m_map[MapIndex_XAxisLeft].type != m_map[MapIndex_XAxisRight].type;
            bool wrong_yaxis = (i == MapIndex_YAxisUp || i == MapIndex_YAxisUp)
                && m_map[MapIndex_YAxisUp].type != m_map[MapIndex_YAxisUp].type;
            axis_mismatch = wrong_xaxis || wrong_yaxis;

            if (axis_mismatch) {
                ImGui::PushStyleColor(ImGuiCol_Text, {0.2f, 0.2f, 0.2f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_Button, {0.7f, 0.7f, 0.16f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.8f, 0.8f, 0.3f, 0.78f});
            }
        }

        if (i == m_map_index) {
            auto col = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
            ImGui::PushStyleColor(ImGuiCol_Button, col);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {col.x + 0.18f, col.y + 0.18f, col.z + 0.18f, col.w - 0.22f});
        }

        if (m_map[i].type == MapType::JoyAxis && i > MapIndex_YAxisDown) {
            if (ImGui::Button(m_map_str[i].c_str(), {127.0f, 0.0f}))
                *next = static_cast<MapIndex>(i);

            ImGui::SameLine();
            ImGui::PushItemWidth(63.0f);

            bool ignore_deadzone = m_map[i].jaxis.deadzone < 0.01f;

            if (ignore_deadzone)
                ImGui::PushStyleColor(ImGuiCol_Text, {0.4f, 0.4f, 0.4f, 1.0f});

            if (ImGui::InputFloat(fmt::format("##d{}", i).c_str(), &m_map[i].jaxis.deadzone, 0.0f, 0.0f, "%.2f%%"))
                m_map[i].jaxis.deadzone = std::clamp(m_map[i].jaxis.deadzone, 0.0f, 100.0f);

            if (ignore_deadzone)
                ImGui::PopStyleColor();

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Axis deadzone, ignored if <0.01%%");

            ImGui::PopItemWidth();
        }
        else {
            if (ImGui::Button(m_map_str[i].c_str(), {200.0f, 0.0f}))
                *next = static_cast<MapIndex>(i);
        }

        if (i == m_map_index) {
            ImGui::SetScrollHereY();
            ImGui::PopStyleColor(2);
        }

        if (axis_mismatch)
            ImGui::PopStyleColor(3);

        ImGui::PopID();
    }
}

void ContTab::AxisStepCheck()
{
    if (m_prev_map.type == MapType::JoyAxis) {
        bool initial = true;

        for (int i{}; i < m_joystick.GetNumAxes(); ++i)
        {
            bool in_range = m_joystick.GetAxis(i) > m_joy_init_axes[i] - 5000
                && m_joystick.GetAxis(i) < m_joy_init_axes[i] + 5000;

            if (!in_range) {
                initial = false;
                break;
            }
        }

        if (initial)
            m_prev_map.type = MapType::None;
    }
}

}

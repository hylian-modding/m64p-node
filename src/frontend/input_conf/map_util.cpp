#include "common/string_util.h"
#include "frontend/input_conf/map_util.h"
#include "frontend/input_conf/sdl_key_converter.h"

#include <array>
#include <unordered_map>

#include <fmt/format.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_joystick.h>

namespace InputConf::MapUtil {

const std::array<const char*, 4> k_HatDirStr {
    "Right", "Left", "Down", "Up"
};

const std::array<const char*, 2> k_AxisDirStr {
    "+", "-"
};

const std::unordered_map<int, const char*> k_KmodStr {
    std::make_pair(KMOD_NONE, "None"),
    {KMOD_LSHIFT, "Left Shift"},
    {KMOD_RSHIFT, "Right Shift"},
    {KMOD_LCTRL, "Left Ctrl"},
    {KMOD_RCTRL, "Right Ctrl"},
    {KMOD_LALT, "Left Alt"},
    {KMOD_RALT, "Right Alt"},
    {KMOD_LGUI, "Left Gui"},
    {KMOD_RGUI, "Right Gui"},
    {KMOD_NUM, "Num Lock"},
    {KMOD_CAPS, "Caps Lock"},
    {KMOD_MODE, "Mode"},
    {KMOD_CTRL, "Ctrl"},
    {KMOD_SHIFT, "Shift"},
    {KMOD_ALT, "Alt"},
    {KMOD_CTRL | KMOD_SHIFT, "Ctrl + Shift"},
    {KMOD_CTRL | KMOD_ALT, "Ctrl + Alt"},
    {KMOD_SHIFT | KMOD_ALT, "Shift + Alt"}
};

bool HatDirFromStr(const std::string& str, JoyHatDir* dir)
{
    if (!dir)
        return false;

    if (StringUtil::StartsWith(str, "Right"))
        *dir = JoyHatDir::Right;
    else if (StringUtil::StartsWith(str, "Left"))
        *dir = JoyHatDir::Left;
    else if (StringUtil::StartsWith(str, "Down"))
        *dir = JoyHatDir::Down;
    else if (StringUtil::StartsWith(str, "Up"))
        *dir = JoyHatDir::Up;
    else
        return false;

    return true;
}

bool AxisDirFromChar(char c, JoyAxisDir* dir)
{
    if (!dir)
        return false;

    if (c == '+')
        *dir = JoyAxisDir::Positive;
    else if (c == '-')
        *dir = JoyAxisDir::Negative;
    else
        return false;

    return true;
}

MapEvent Parse(const std::string& str)
{
    MapEvent e{};

    int key_index, jbutton_index, jhat_index, jaxis_index;
    char jhat_dir[20];
    char jaxis_dir;
    int jaxis_deadzone;

    if (StringUtil::StartsWith(str, "key")
        && std::sscanf(str.c_str(), "key(%d)", &key_index) == 1) {

        e.type = MapType::Key;
        e.key.scancode = static_cast<SDL_Scancode>(sdl_keysym2scancode(key_index));
    }
    else if (StringUtil::StartsWith(str, "button")
        && std::sscanf(str.c_str(), "button(%d)", &jbutton_index) == 1) {

        e.type = MapType::JoyButton;
        e.jbutton.button = jbutton_index;
    }
    else if (StringUtil::StartsWith(str, "hat")
        && std::sscanf(str.c_str(), "hat(%d %s", &jhat_index, jhat_dir) == 2
        && HatDirFromStr(jhat_dir, &e.jhat.dir)) {

        e.type = MapType::JoyHat;
        e.jhat.hat = jhat_index;
    }
    else if (StringUtil::StartsWith(str, "axis")) {
        if (std::sscanf(str.c_str(), "axis(%d%c,%d)", &jaxis_index, &jaxis_dir, &jaxis_deadzone) == 3
            && AxisDirFromChar(jaxis_dir, &e.jaxis.dir)) {

            e.type = MapType::JoyAxis;
            e.jaxis.axis = jaxis_index;
            e.jaxis.deadzone = ShortToPercent(jaxis_deadzone);
        }
        else if (std::sscanf(str.c_str(), "axis(%d%c)", &jaxis_index, &jaxis_dir) == 2
            && AxisDirFromChar(jaxis_dir, &e.jaxis.dir)) {

            e.type = MapType::JoyAxis;
            e.jaxis.axis = jaxis_index;
            e.jaxis.deadzone = 0.0f;
        }
    }

    return e;
}

MapEventPair ParsePair(const std::string& str)
{
    MapEventPair e{};

    int key_index1, key_index2;
    int jbutton_index1, jbutton_index2;
    int jhat_index;
    char jhat_dir1[20], jhat_dir2[20];
    int jaxis_index1, jaxis_index2;
    char jaxis_dir1, jaxis_dir2;

    if (StringUtil::StartsWith(str, "key")
        && std::sscanf(str.c_str(), "key(%d,%d)", &key_index1, &key_index2) == 2) {

        e.first.type = e.second.type = MapType::Key;
        e.first.key.scancode = static_cast<SDL_Scancode>(sdl_keysym2scancode(key_index1));
        e.second.key.scancode = static_cast<SDL_Scancode>(sdl_keysym2scancode(key_index2));
    }
    else if (StringUtil::StartsWith(str, "button")
        && std::sscanf(str.c_str(), "button(%d,%d)", &jbutton_index1, &jbutton_index2) == 2) {

        e.first.type = e.second.type = MapType::JoyButton;
        e.first.jbutton.button = jbutton_index1;
        e.second.jbutton.button = jbutton_index2;
    }
    else if (StringUtil::StartsWith(str, "hat")
        && std::sscanf(str.c_str(), "hat(%d %s %s", &jhat_index, jhat_dir1, jhat_dir2) == 3
        && HatDirFromStr(jhat_dir1, &e.first.jhat.dir)
        && HatDirFromStr(jhat_dir2, &e.second.jhat.dir)) {

        e.first.type = e.second.type = MapType::JoyHat;
        e.first.jhat.hat = e.second.jhat.hat = jhat_index;
    }
    else if (StringUtil::StartsWith(str, "axis")
        && std::sscanf(str.c_str(), "axis(%d%c,%d%c)", &jaxis_index1, &jaxis_dir1, &jaxis_index2, &jaxis_dir2) == 4
        && AxisDirFromChar(jaxis_dir1, &e.first.jaxis.dir)
        && AxisDirFromChar(jaxis_dir2, &e.second.jaxis.dir)) {

        e.first.type = e.second.type = MapType::JoyAxis;
        e.first.jaxis.axis = jaxis_index1;
        e.second.jaxis.axis = jaxis_index2;
        e.first.jaxis.deadzone = e.second.jaxis.deadzone = 0.0f;
    }

    return e;
}

std::string Format(const MapEvent& e, bool pretty_str)
{
    if (pretty_str) {
        switch (e.type) {
        default:
            return "None";
        case MapType::Key:
            return SDL_GetKeyName(SDL_GetKeyFromScancode(e.key.scancode));
        case MapType::JoyButton:
            return fmt::format("Button {}", e.jbutton.button);
        case MapType::JoyHat:
            return fmt::format("Hat {}, {}", e.jhat.hat, k_HatDirStr[static_cast<std::size_t>(e.jhat.dir)]);
        case MapType::JoyAxis:
            return fmt::format("Axis {}{}", e.jaxis.axis, k_AxisDirStr[static_cast<std::size_t>(e.jaxis.dir)]);
        }
    }
    else {
        switch (e.type) {
        default:
            return "";
        case MapType::Key:
            return fmt::format("key({})", sdl_scancode2keysym(e.key.scancode));
        case MapType::JoyButton:
            return fmt::format("button({})", e.jbutton.button);
        case MapType::JoyHat:
            return fmt::format("hat({} {})", e.jhat.hat, k_HatDirStr[static_cast<std::size_t>(e.jhat.dir)]);
        case MapType::JoyAxis:
            return e.jaxis.deadzone >= 0.01f ?
                fmt::format("axis({}{},{})",
                e.jaxis.axis, k_AxisDirStr[static_cast<std::size_t>(e.jaxis.dir)], ShortFromPercent(e.jaxis.deadzone)) :
                fmt::format("axis({}{})",
                e.jaxis.axis, k_AxisDirStr[static_cast<std::size_t>(e.jaxis.dir)]);
        }
    }
}

std::string FormatPair(const MapEventPair& e)
{
    if (e.first.type != e.second.type)
        return "";

    switch (e.first.type) {
    default:
        return "";
    case MapType::Key:
        return fmt::format("key({},{})",
            sdl_scancode2keysym(e.first.key.scancode),
            sdl_scancode2keysym(e.second.key.scancode));
    case MapType::JoyButton:
        return fmt::format("button({},{})",
            e.first.jbutton.button,
            e.second.jbutton.button);
    case MapType::JoyHat:
        return fmt::format("hat({} {} {})",
            e.first.jhat.hat, k_HatDirStr[static_cast<std::size_t>(e.first.jhat.dir)],
            k_HatDirStr[static_cast<std::size_t>(e.second.jhat.dir)]);
    case MapType::JoyAxis:
        return fmt::format("axis({}{},{}{})",
            e.first.jaxis.axis, k_AxisDirStr[static_cast<std::size_t>(e.first.jaxis.dir)],
            e.second.jaxis.axis, k_AxisDirStr[static_cast<std::size_t>(e.second.jaxis.dir)]);
    }
}

MapEvent ParseKbdEvent(const std::string& str)
{
    MapEvent e{};
    int key;

    if (std::sscanf(str.c_str(), "%d", &key) == 1) {
        if (key > 0) {
            e.type = MapType::Key;
            e.key.scancode =  static_cast<SDL_Scancode>(sdl_keysym2scancode(key & 0xffff));
            e.key.mod = (key >> 16) & 0xffff;
        }
    }

    return e;
}

std::string FormatKbdEvent(const MapEvent& e, bool pretty_str)
{
    if (pretty_str) {
        auto key_name =  SDL_GetKeyName(SDL_GetKeyFromScancode(e.key.scancode));

        return e.type == MapType::Key ?
            e.key.mod == KMOD_NONE ? key_name : fmt::format("{} + {}", k_KmodStr.at(e.key.mod), key_name)
            : "None";
    }
    else {
        return e.type == MapType::Key ?
            fmt::format("{}", (e.key.mod << 16) | sdl_scancode2keysym(e.key.scancode))
            : "";
    }
}

int GetKbdEventKey(const MapEvent& e)
{
    if (e.type == MapType::Key && e.key.scancode > 0)
        return (e.key.mod << 16) | sdl_scancode2keysym(e.key.scancode);
    else
        return 0;
}

bool IsSame(const MapEvent& lhs, const MapEvent& rhs)
{
    if (lhs.type != rhs.type)
        return false;

    if (lhs.type == MapType::Key)
        return lhs.key.scancode == rhs.key.scancode;
    else if (lhs.type == MapType::JoyButton)
        return lhs.jbutton.button == rhs.jbutton.button;
    else if (lhs.type == MapType::JoyHat)
        return lhs.jhat.hat == rhs.jhat.hat && lhs.jhat.dir == rhs.jhat.dir;
    else if (lhs.type == MapType::JoyAxis)
        return lhs.jaxis.axis == rhs.jaxis.axis && lhs.jaxis.dir == rhs.jaxis.dir;

    return false;
}

}

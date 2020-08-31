#pragma once

#include <string>
#include <utility>

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>

namespace InputConf::MapUtil {

enum class MapType {
	None, Key, JoyButton, JoyHat, JoyAxis
};

enum class JoyHatDir {
	Right, Left, Down, Up
};

enum class JoyAxisDir {
	Positive, Negative
};

struct KeyMap {
	MapType type;
	SDL_Scancode scancode;
	Uint16 mod;
};

struct JoyButtonMap {
	MapType type;
	int button;
	Sint32 which;
};

struct JoyHatMap {
	MapType type;
	int hat;
	JoyHatDir dir;
	Sint32 which;
	int raw_dir;
};

struct JoyAxisMap {
	MapType type;
	int axis;
	float deadzone;
	JoyAxisDir dir;
	Sint32 which;
};

union MapEvent {
	MapType type;
	KeyMap key;
	JoyButtonMap jbutton;
	JoyHatMap jhat;
	JoyAxisMap jaxis;
};

using MapEventPair = std::pair<MapEvent, MapEvent>;

MapEvent Parse(const std::string& str);
MapEventPair ParsePair(const std::string& str);
std::string Format(const MapEvent& e, bool pretty_str);
std::string FormatPair(const MapEventPair& e);
MapEvent ParseKbdEvent(const std::string& str);
std::string FormatKbdEvent(const MapEvent& e, bool pretty_str);
int GetKbdEventKey(const MapEvent& e);
bool IsSame(const MapEvent& lhs, const MapEvent& rhs);

enum MapIndex {
	MapIndex_None = -1,
	MapIndex_XAxisLeft,
	MapIndex_XAxisRight,
	MapIndex_YAxisUp,
	MapIndex_YAxisDown,
	MapIndex_DPadLeft,
	MapIndex_DPadRight,
	MapIndex_DPadUp,
	MapIndex_DPadDown,
	MapIndex_Start,
	MapIndex_TrigZ,
	MapIndex_B,
	MapIndex_A,
	MapIndex_CLeft,
	MapIndex_CRight,
	MapIndex_CUp,
	MapIndex_CDown,
	MapIndex_TrigL,
	MapIndex_TrigR,
	MapIndex_CLeft2,
	MapIndex_CRight2,
	MapIndex_CUp2,
	MapIndex_CDown2,
	MapIndex_MempakSwitch,
	MapIndex_RumbleSwitch,
	MapIndex_Count
};

enum EventIndex {
	EventIndex_None = -1,
	EventIndex_KbdStop,
	EventIndex_KbdFullScreen,
	EventIndex_KbdSaveState,
	EventIndex_KbdLoadState,
	EventIndex_KbdIncState,
	EventIndex_KbdReset,
	EventIndex_KbdSpeedDown,
	EventIndex_KbdSpeedUp,
	EventIndex_KbdScreenshot,
	EventIndex_KbdPause,
	EventIndex_KbdMute,
	EventIndex_KbdIncVolume,
	EventIndex_KbdDecVolume,
	EventIndex_KbdFastForward,
	EventIndex_KbdFrameAdvance,
	EventIndex_KbdGameShark,
	EventIndex_JoyStop,
	EventIndex_JoyFullScreen,
	EventIndex_JoySaveState,
	EventIndex_JoyLoadState,
	EventIndex_JoyIncState,
	EventIndex_JoyReset,
	EventIndex_JoySpeedDown,
	EventIndex_JoySpeedUp,
	EventIndex_JoyScreenshot,
	EventIndex_JoyPause,
	EventIndex_JoyMute,
	EventIndex_JoyIncVolume,
	EventIndex_JoyDecVolume,
	EventIndex_JoyFastForward,
	EventIndex_JoyFrameAdvance,
	EventIndex_JoyGameShark,
	EventIndex_Count,
	EventIndex_NumEvents = 16
};

inline int ShortFromPercent(float v)
{
	return static_cast<int>(v / 100.0f * 32768.0f);
}

inline float ShortToPercent(int v)
{
	return v / 32768.0f * 100.0f;
}

}

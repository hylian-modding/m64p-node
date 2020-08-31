#pragma once

#include "frontend/input_conf/map_util.h"
#include "m64p/fwd.h"

#include <array>
#include <string>

union SDL_Event;

namespace InputConf {

class EventsTab
{
public:
	EventsTab();

	void LoadConfig(M64P::Core& core);
    void SaveConfig(M64P::Core& core);
	void DoEvent(const SDL_Event& e);
	void Show();

private:
	std::array<MapUtil::MapEvent, MapUtil::EventIndex_NumEvents> m_kbd_map{};
	std::array<std::string, MapUtil::EventIndex_NumEvents> m_kbd_map_str;
	MapUtil::EventIndex m_map_index{ MapUtil::EventIndex_None };

    void AdvanceMapIndex();
	void ShowMapButtons(MapUtil::EventIndex* next, MapUtil::EventIndex min, MapUtil::EventIndex max);
};

}

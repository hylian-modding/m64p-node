#pragma once

#include "frontend/input_conf/map_util.h"
#include "sdl/joystick.h"
#include "m64p/fwd.h"

#include <array>
#include <string>
#include <vector>

union SDL_Event;

namespace InputConf {

class ContTab
{
public:
	ContTab(int cont_index);

    void LoadConfig(M64P::Core& core);
    void SaveConfig(M64P::Core& core);
	void DoEvent(const SDL_Event& e, bool focused);
	void Show();

private:
	int m_cont_index{};
	std::vector<std::string> m_devices;
	int m_device_index{};
	SDL::Joystick m_joystick;
	std::vector<Sint16> m_joy_init_axes;
	bool m_plugged{};
	int m_plugin_index{};
	std::array<float, 2> m_ana_deadzones{};
	std::array<float, 2> m_ana_peaks{};
	std::array<MapUtil::MapEvent, MapUtil::MapIndex_Count> m_map{};
	std::array<std::string, MapUtil::MapIndex_Count> m_map_str;
	MapUtil::MapIndex m_map_index{ MapUtil::MapIndex_None };
	MapUtil::MapEvent m_prev_map{};
	bool m_key_map{};

    void RememberDevice(int device_index, const std::string& device_name);
    void AdvanceMapIndex();
	void UpdateDeviceList();
	void UpdateDeviceIndex(std::string device_name);
	void UpdateDeviceListAndIndex();
	void OpenDeviceIndex(int device_index);
	void ShowDevicesCombo();
	void ShowPluginsCombo();
	void ShowMapButtons(MapUtil::MapIndex* next, MapUtil::MapIndex min, MapUtil::MapIndex max, bool axis_mismatch_check = false);
    void AxisStepCheck();
};

}

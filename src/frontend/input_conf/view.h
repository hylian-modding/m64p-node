#pragma once

#include "frontend/input_conf/cont_tab.h"
#include "frontend/input_conf/events_tab.h"
#include "frontend/tool_base.h"
#include "m64p/fwd.h"

namespace InputConf {

class View : public Frontend::ToolBase {
public:
    View();

    void LoadConfig(M64P::Core& core);
    void SaveConfig(M64P::Core& core);
    void DoEvent(const SDL_Event& e);
    void Show(SDL::Window& main_win);

private:
    std::size_t m_tab_index{};
    std::array<ContTab, 4> m_cont_tabs;
    EventsTab m_events_tab;
};

}

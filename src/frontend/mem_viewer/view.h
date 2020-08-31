#pragma once

#include "frontend/tool_base.h"
#include "m64p/fwd.h"

struct ImFont;

namespace MemViewer {

class View : public Frontend::ToolBase {
public:
    View(M64P::Core& core);

    void Show(SDL::Window& main_win, ImFont* font_mono);

private:
    M64P::Core* m_core;
};

}

#pragma once

#include "frontend/tool_base.h"
#include "m64p/fwd.h"

struct ImFont;

namespace MemViewer {

class MemViewerImpl;

class View : public Frontend::ToolBase {
public:
    View(M64P::Core& core);
    ~View();

    void Show(SDL::Window& main_win, ImFont* font_mono);

private:
    std::unique_ptr<MemViewerImpl> m_impl;
};

}

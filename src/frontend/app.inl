#include "frontend/app.h"

namespace Frontend {

inline App& App::GetInstance()
{
    static App app;
    return app;
}

inline M64P::Core& App::GetCore()
{
    return m_emu.core;
}

inline M64P::Plugin& App::GetPlugin(m64p_plugin_type type)
{
    switch (type) {
    case M64PLUGIN_GFX:
        return m_emu.plugins[0];
    case M64PLUGIN_AUDIO:
        return m_emu.plugins[1];
    case M64PLUGIN_INPUT:
        return m_emu.plugins[2];
    case M64PLUGIN_RSP:
        return m_emu.plugins[3];
    default:
        throw std::runtime_error{"bad plugin type"};
    }
}

inline Input& App::GetInput()
{
    return m_input;
}

inline bool App::IsDebuggerSupported()
{
    return (m_emu.core.GetCaps() & M64CAPS_DEBUGGER) == M64CAPS_DEBUGGER;
}

inline bool App::IsDebuggerEnabled()
{
    return m_emu.core.ConfigOpenSection("Core").GetBoolOr("EnableDebugger", false);
}

inline bool App::IsDebuggerInitialized() const
{
    return m_emu.debug_init;
}

inline void App::CreateResourcesNextVi()
{
    m_create_res_next = true;
}

inline const FrameCaptureInfo& App::GetFrameCaptureInfo() const
{
    return m_capture;
}

inline void App::WantRebuildFontAtlas()
{
    m_fonts.rebuild_atlas = true;
}

inline ImFont* App::GetDefaultFont() const
{
    return m_fonts.base;
}

inline void App::SetDefaultFont(ImFont* font)
{
    m_fonts.base = font;
}

inline ImFont* App::GetDefaultMonoFont() const
{
    return m_fonts.mono;
}

inline void App::SetDefaultMonoFont(ImFont* font)
{
    m_fonts.mono = font;
}

inline void App::OverrideCheatsCrc(std::string crc)
{
    m_cheats.crc = std::move(crc);
}

inline InputConf::View* App::GetInputConf() const
{
    return m_util_win.input_conf.get();
}

inline CheatConf::View* App::GetCheatConf() const
{
    return m_util_win.cheat_conf.get();
}

inline MemViewer::View* App::GetMemViewer() const
{
    return m_util_win.mem_viewer.get();
}

inline SDL::Window& App::GetMainWindow()
{
    return m_video.window;
}

inline bool App::HasEmuInputFocus() const
{
    return !m_any_item_active && m_video.window.HasInputFocus();
}

inline u64 App::GetElapsedFrameCount() const
{
    return m_emu.elapsed_frames;
}

inline void App::TakeNextScreenshot()
{
    m_take_shot = true;
}

}

#pragma once

#include "frontend/cheat_conf/fwd.h"
#include "frontend/input_conf/fwd.h"
#include "frontend/mem_viewer/fwd.h"
#include "frontend/imgui_ctx.h"
#include "frontend/input.h"
#include "m64p/fwd.h"
#include "m64p/cheat.h"
#include "m64p/core.h"
#include "m64p/plugin.h"
#include "sdl/gl_context.h"
#include "sdl/img_init.h"
#include "sdl/mix_device.h"
#include "sdl/mix_init.h"
#include "sdl/sdl_init.h"
#include "sdl/ttf_init.h"
#include "sdl/window.h"

#include <array>
#include <filesystem>
#include <future>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

#include <m64p_api/m64p_types.h>
#include <nano_signal_slot.hpp>

struct ImFont;

namespace Frontend {

struct StartInfo {
    std::string window_title;
    int window_width;
    int window_height;
    std::vector<u8> window_icon;
    std::filesystem::path core_plugin;
    std::filesystem::path rsp_plugin;
    std::filesystem::path gfx_plugin;
    std::filesystem::path audio_plugin;
    std::filesystem::path input_plugin;
    std::filesystem::path config_dir;
    std::filesystem::path data_dir;
};

struct VideoOutputInfo {
    float screen_width;
    float screen_height;
    float left;
    float top;
    float width;
    float height;
};

struct FrameCaptureInfo {
    u32 texture_id;
    int width, height;
    std::vector<u8> pixels;
};

class App {
    friend struct AppCallbacks;
    friend struct VidExtCallbacks;

public:
    Nano::Signal<void()> OnCoreStarted;
    Nano::Signal<void()> OnCoreStopped;
    Nano::Signal<void()> OnNewFrame;
    Nano::Signal<void()> OnNewVI;
    Nano::Signal<void(bool do_hard_reset)> OnCoreReset;
    Nano::Signal<void(int event, int data)> OnCoreEvent;
    Nano::Signal<void(m64p_core_param param_type, int new_value)> OnCoreStateChanged;
    Nano::Signal<void()> OnDebugInit;
    Nano::Signal<void(unsigned pc)> OnDebugUpdate;
    Nano::Signal<void()> OnCreateResources;
    Nano::Signal<void()> OnViUpdate;
    Nano::Signal<void()> OnWindowClosing;

    static App& GetInstance();
    M64P::Core& GetCore();
    M64P::Plugin& GetPlugin(m64p_plugin_type type);
    Input& GetInput();
    bool IsDebuggerSupported();
    bool IsDebuggerEnabled();
    bool IsDebuggerInitialized() const;
    void CreateResourcesNextVi();
    const FrameCaptureInfo& GetFrameCaptureInfo() const;
    void WantRebuildFontAtlas();
    ImFont* GetDefaultFont() const;
    void SetDefaultFont(ImFont* font);
    ImFont* GetDefaultMonoFont() const;
    void SetDefaultMonoFont(ImFont* font);
    void OverrideCheatsCrc(std::string crc);
    InputConf::View* GetInputConf() const;
    CheatConf::View* GetCheatConf() const;
    MemViewer::View* GetMemViewer() const;
    SDL::Window& GetMainWindow();
    bool HasEmuInputFocus() const;
    u64 GetElapsedFrameCount() const;
    void TakeNextScreenshot();

    void Startup(const StartInfo& info);
    void Shutdown();
    void Execute();
    void Stop();
    VideoOutputInfo GetVideoOutputInfo();
    void ToggleFullScreen();
    void CaptureFrame();
    void DispatchEvents();
    void DestroyTextureLater(u32 id);
    void BindingBeforeCreateResources();
    void BindingAfterCreateResources();
    void BindingBeforeRender();
    void BindingAfterRender();

private:
    struct SdlInit {
        SDL::SDLInit sdl;
        SDL::IMGInit img;
        SDL::TTFInit ttf;
        SDL::MixInit mix;
    };

    struct Video {
        SDL::Window window;
        SDL::GLContext gl_context;
        ImGuiCtx imgui;
    };

    struct Emu {
        M64P::Core core;
        std::array<M64P::Plugin, 4> plugins;
        std::filesystem::path data_dir;
        std::future<void> execute;
        u64 elapsed_frames;
        int gfx_aspect;
        std::mutex states_mutex;
        std::vector<std::pair<m64p_core_param, int>> states;
        std::mutex events_mutex;
        std::vector<int> events;
        int notify_reset;
        bool core_init;
        bool debug_init;
        bool stopping;
        bool started;
        bool notify_started;
    };

    struct Cheats {
        M64P::Cheat::BlockMap map;
        M64P::Cheat::BlockMap user_map;
        M64P::Cheat::Block* block;
        M64P::Cheat::Block* user_block;
        std::string crc;
    };

    struct UtilWindows {
        std::unique_ptr<InputConf::View> input_conf;
        std::unique_ptr<CheatConf::View> cheat_conf;
        std::unique_ptr<MemViewer::View> mem_viewer;
    };

    struct Fonts {
        ImFont* base;
        ImFont* mono;
        bool rebuild_atlas;
    };

    SdlInit m_sdl_init;
    SDL::MixDevice m_mix_dev;
    Video m_video;
    Emu m_emu{};
    Input m_input;
    Cheats m_cheats{};
    UtilWindows m_util_win;
    Fonts m_fonts{};
    FrameCaptureInfo m_capture{};
    bool m_any_item_active{};
    bool m_create_res_next{};
    std::mutex m_tex_to_destroy_mutex;
    std::vector<u32> m_tex_to_destroy;
    bool m_take_shot{};

    App();
    ~App();

    void InitVideo(const StartInfo& info);
    void DeinitVideo();
    void InitFrameCapture();
    void InitEmu(const StartInfo& info);
    void DeinitEmu();
    void DoEvents();
    void LoadCheats();
    void SaveCheats();
    void LoadROMCheats();
    void InitUtilWindows();
    void DeinitUtilWindows();
    void InputConfigClosedHandler();
    void ShowUtilWindows();
    void UpdateVideoOutputSize();
    void DispatchCoreEvents();
    void DispatchCoreStates();
    void DestroyTextures();
    void CoreStartedHandler();
    void CoreStoppedHandler();
    void CreateResourcesHandler();
    void NewFrameHandler();
    void NewVIHandler();
    void ResetHandler(bool do_hard_reset);
    void PauseLoopHandler();
    void CoreEventHandler(int event);
    void CoreStateChangedHandler(m64p_core_param param_type, int new_value);
    void DebugInitHandler();
    void DebugUpdateHandler(unsigned pc);
    void SwapHandler();
    std::filesystem::path GetScreenshotPath();
    void TakeScreenshot();
};

}

#include "app.inl"

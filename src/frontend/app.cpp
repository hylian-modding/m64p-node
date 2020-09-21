#include "common/logged_runtime_error.h"
#include "frontend/cheat_conf/view.h"
#include "frontend/fonts/fonts.h"
#include "frontend/gfx/texture.h"
#include "frontend/input_conf/view.h"
#include "frontend/mem_viewer/view.h"
#include "frontend/app.h"
#include "frontend/vidext.h"
#include "imgui/imgui.h"
#include "sdl/surface.h"

#include <chrono>
#include <iostream>
#include <sstream>

#include <fmt/format.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace Frontend {

App::App()
{
    SDL_SetMainReady();

    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");

    m_sdl_init.sdl.Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
    m_sdl_init.img.Init(IMG_INIT_PNG | IMG_INIT_JPG);
    m_sdl_init.ttf.Init();
}

App::~App()
{
    m_sdl_init.ttf.Quit();
    m_sdl_init.img.Quit();
    m_sdl_init.sdl.Quit();
}

void App::InitVideo(const StartInfo& info)
{
    m_video.window = {info.window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, info.window_width, info.window_height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN};

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

    m_video.gl_context = SDL::GLContext{m_video.window};

    if (glewInit() != GLEW_OK)
        throw LoggedRuntimeError{"M64p frontend", "Failed to initialize GLEW"};

    m_video.imgui.Initialize(m_video.window, m_video.gl_context);

    if (!info.window_icon.empty())
        m_video.window.SetIcon({info.window_icon.data(), info.window_icon.size()});

    auto font_atlas = ImGui::GetIO().Fonts;
    m_fonts.base = font_atlas->AddFontFromMemoryCompressedTTF(DroidSans_compressed_data, DroidSans_compressed_size, 18);
    m_fonts.mono = font_atlas->AddFontFromMemoryCompressedTTF(DroidSansMono_compressed_data, DroidSansMono_compressed_size, 18);
    m_video.imgui.RebuildFontAtlas();
}

void App::DeinitVideo()
{
    m_video.imgui.Deinitialize();
    m_video.gl_context = {};
    m_video.window = {};
}

void App::InitFrameCapture()
{
    glGenTextures(1, &m_capture.texture_id);
    glBindTexture(GL_TEXTURE_2D, m_capture.texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

inline void DebugLog(void*, int level, const char* message)
{
    switch (level) {
    case M64MSG_ERROR:
        Logger::Log(LogCategory::Error, "M64p", message);
        break;
    case M64MSG_WARNING:
        Logger::Log(LogCategory::Warn, "M64p", message);
        break;
    case M64MSG_INFO:
        Logger::Log(LogCategory::Info, "M64p", message);
        break;
    case M64MSG_STATUS:
        Logger::Log(LogCategory::Trace, "M64p", message);
        break;
    }
}

struct AppCallbacks {
    static void NewFrameHandler()
    {
        App::GetInstance().NewFrameHandler();
    }

    static void NewVIHandler()
    {
        App::GetInstance().NewVIHandler();
    }

    static void ResetHandler(int do_hard_reset)
    {
        App::GetInstance().ResetHandler(do_hard_reset);
    }

    static void PauseLoopHandler()
    {
        App::GetInstance().PauseLoopHandler();
    }

    static void CoreEventHandler(int event)
    {
        App::GetInstance().CoreEventHandler(event);
    }

    static void CoreStateChangedHandler(void*, m64p_core_param param_type, int new_value)
    {
        App::GetInstance().CoreStateChangedHandler(param_type, new_value);
    }

    static void DebugInitHandler()
    {
        App::GetInstance().DebugInitHandler();
    }

    static void DebugUpdateHandler(unsigned pc)
    {
        App::GetInstance().DebugUpdateHandler(pc);
    }
};

void App::InitEmu(const StartInfo& info)
{
    m_emu.core.Load(info.core_plugin);
    m_emu.core.Startup(0x020001, info.config_dir, info.data_dir, nullptr, DebugLog, nullptr, AppCallbacks::CoreStateChangedHandler);
    m_emu.data_dir = info.data_dir;

    m64p_video_extension_functions vidext_func;
    VidExt::GetFunctions(&vidext_func);
    m_emu.core.OverrideVidExt(vidext_func);

    auto video_gen_conf = m_emu.core.ConfigOpenSection("Video-General");
    video_gen_conf.SetBool("Fullscreen", false);
    video_gen_conf.SetInt("ScreenWidth", info.window_width);
    video_gen_conf.SetInt("ScreenHeight", info.window_height);
    video_gen_conf.SetBool("VerticalSync", false);
    video_gen_conf.Save();

    GetPlugin(M64PLUGIN_GFX).Load(info.gfx_plugin);
    GetPlugin(M64PLUGIN_AUDIO).Load(info.audio_plugin);
    GetPlugin(M64PLUGIN_INPUT).Load(info.input_plugin);
    GetPlugin(M64PLUGIN_RSP).Load(info.rsp_plugin);

    m_emu.core.SetFrameCallback(AppCallbacks::NewFrameHandler);
    m_emu.core.SetVICallback(AppCallbacks::NewVIHandler);
    m_emu.core.SetResetCallback(AppCallbacks::ResetHandler);
    m_emu.core.SetPauseLoopCallback(AppCallbacks::PauseLoopHandler);
    m_emu.core.SetEventCallback(AppCallbacks::CoreEventHandler);

    if (IsDebuggerSupported())
        m_emu.core.DebugSetCallbacks(AppCallbacks::DebugInitHandler, AppCallbacks::DebugUpdateHandler, []() {});

    m_emu.core_init = true;
}

void App::DeinitEmu()
{
    for (auto& plugin: m_emu.plugins)
        plugin.Unload();

    m_emu.core.ConfigSaveFile();
    m_emu.core.Shutdown();
    m_emu.core.Unload();

    m_emu.core_init = false;
    m_emu.debug_init = false;
    m_emu.started = false;
}

void App::DoEvents()
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        m_video.imgui.ProcessEvent(e);

        if (m_emu.core_init) {
            if (e.type == SDL_WINDOWEVENT && e.window.windowID == m_video.window.GetId()) {
                if (e.window.event == SDL_WINDOWEVENT_CLOSE)
                    OnWindowClosing.fire();
            }
            else if (!m_any_item_active && e.key.windowID == m_video.window.GetId() && m_video.window.HasInputFocus()) {
                if (e.type == SDL_KEYDOWN)
                    m_emu.core.InputKeyDown(e.key.keysym.mod, e.key.keysym.scancode);
                else if (e.type == SDL_KEYUP)
                    m_emu.core.InputKeyUp(e.key.keysym.mod, e.key.keysym.scancode);
            }
        }

        if (m_util_win.input_conf)
            m_util_win.input_conf->DoEvent(e);
    }
}

void App::LoadCheats()
{
    static const std::string k_NullCrc = "00000000-00000000-0:00";

    static const auto init_block = [](M64P::Cheat::Block* block) {
        block->crc = k_NullCrc;
        block->good_name = "None";
    };

    try {
        m_cheats.map = M64P::Cheat::Load(m_emu.data_dir / "mupencheat.txt");
    }
    catch (const std::exception& e) {
        Logger::Log(LogCategory::Warn, "M64p frontend", fmt::format("Cheats not loaded. {}", e.what()));
    }

    try {
        m_cheats.user_map = M64P::Cheat::Load(m_emu.data_dir / "mupencheat_user.txt");
    }
    catch (const std::exception& e) {
        Logger::Log(LogCategory::Warn, "M64p frontend", fmt::format("User cheats not loaded. {}", e.what()));
    }

    m_cheats.block = &m_cheats.map[k_NullCrc];
    init_block(m_cheats.block);
    m_cheats.user_block = &m_cheats.user_map[k_NullCrc];
    init_block(m_cheats.user_block);
}

void App::SaveCheats()
{
    try {
        M64P::Cheat::Save(m_emu.data_dir / "mupencheat_user.txt", m_cheats.user_map);
    }
    catch (const std::exception& e) {
        Logger::Log(LogCategory::Error, "M64p frontend", fmt::format("User cheats not saved. {}", e.what()));
    }
}

void App::LoadROMCheats()
{
    m64p_rom_header header;
    m_emu.core.GetROMHeader(&header);
    m64p_rom_settings settings;
    m_emu.core.GetROMSettings(&settings);

    auto crc = !m_cheats.crc.empty() ? m_cheats.crc :
        fmt::format("{:08X}-{:08X}-C:{:02X}", SDL_Swap32(header.CRC1), SDL_Swap32(header.CRC2), header.Country_code & 0xff);

    if (auto it = m_cheats.map.find(crc); it != m_cheats.map.end()) {
        m_cheats.block = &it->second;
    }
    else {
        m_cheats.block = &m_cheats.map[crc];
        m_cheats.block->crc = crc;
        m_cheats.block->good_name = settings.goodname;
    }

    if (auto it = m_cheats.user_map.find(crc); it != m_cheats.user_map.end()) {
        m_cheats.user_block = &it->second;
    }
    else {
        m_cheats.user_block = &m_cheats.user_map[crc];
        m_cheats.user_block->crc = crc;
        m_cheats.user_block->good_name = settings.goodname;
    }
}

void App::InitUtilWindows()
{
    m_util_win.input_conf = std::make_unique<InputConf::View>();
    m_util_win.input_conf->LoadConfig(m_emu.core);
    m_util_win.input_conf->OnClosed.connect<&App::InputConfigClosedHandler>(this);

    m_util_win.cheat_conf = std::make_unique<CheatConf::View>(m_emu.core, *m_cheats.block, *m_cheats.user_block);

    m_util_win.mem_viewer = std::make_unique<MemViewer::View>(m_emu.core);
}

void App::DeinitUtilWindows()
{
    m_util_win.input_conf->OnClosed.disconnect<&App::InputConfigClosedHandler>(this);
    m_util_win.input_conf->SaveConfig(m_emu.core);
    m_util_win.input_conf.reset();

    m_util_win.cheat_conf->DisableAllEntries();
    m_util_win.cheat_conf.reset();

    m_util_win.mem_viewer.reset();
}

void App::InputConfigClosedHandler()
{
    m_util_win.input_conf->SaveConfig(m_emu.core);
    m_emu.core.ResetInputPlugin();
}

void App::ShowUtilWindows()
{
    // ImGui::GetIO().ConfigViewportsNoDecoration = false;

    if (m_util_win.input_conf)
        m_util_win.input_conf->Show(m_video.window);

    if (m_util_win.cheat_conf)
        m_util_win.cheat_conf->Show(m_video.window, m_fonts.mono);

    if (m_util_win.mem_viewer)
        m_util_win.mem_viewer->Show(m_video.window, m_fonts.mono);

    // ImGui::GetIO().ConfigViewportsNoDecoration = true;
}

void App::Startup(const StartInfo& info)
{
    InitVideo(info);
    InitFrameCapture();
    InitEmu(info);
    LoadCheats();
}

void App::Shutdown()
{
    SaveCheats();
    DeinitEmu();
    DeinitVideo();
}

void App::Execute()
{
    SDL::GLContext::MakeCurrentNone();

    m_emu.execute = std::async(std::launch::async, [this]() {
        try {
            m_video.gl_context.MakeCurrent(m_video.window);
            m_create_res_next = true;

            for (auto& plugin: m_emu.plugins) {
                plugin.Startup(m_emu.core, nullptr, DebugLog);
                m_emu.core.AttachPlugin(plugin);
            }

            CoreStartedHandler();
            m_emu.core.Execute();
            CoreStoppedHandler();

            for (auto& plugin: m_emu.plugins) {
                m_emu.core.DetachPlugin(plugin);
                plugin.Shutdown();
            }

            m_emu.core.CloseROM();

            m_video.gl_context.MakeCurrent(m_video.window);
        }
        catch (const std::exception& e) {
            Logger::Log(LogCategory::Fatal, "M64p frontend", fmt::format("Caught unhandled exception during emulation. {}", e.what()));
            std::exit(EXIT_FAILURE);
        }
        catch (...) {
            Logger::Log(LogCategory::Fatal, "M64p frontend", "Caught unhandled exception during emulation");
            std::exit(EXIT_FAILURE);
        }
    });
}

void App::Stop()
{
    m_emu.core.Stop();
    m_emu.stopping = true;
}

VideoOutputInfo App::GetVideoOutputInfo()
{
    if (m_emu.core.GetEmuState() == M64EMU_STOPPED)
        return {};

    VideoOutputInfo info{};

    auto size = m_emu.core.GetVideoSize();
    info.screen_width = static_cast<float>(size.uiWidth);
    info.screen_height = static_cast<float>(size.uiHeight);

    switch (m_emu.gfx_aspect) {
    default:
    case 0: // stretch
    case 3: // adjust
        info.width = info.screen_width;
        info.height = info.screen_height;
        break;
    case 1: // force 4:3
        if (info.screen_width * 3 / 4 > info.screen_height) {
			info.height = info.screen_height;
			info.width = info.screen_height * 4 / 3;
            info.left = (info.screen_width - info.width) / 2;
		}
        else if (info.screen_height * 4 / 3 > info.screen_width) {
			info.width = info.screen_width;
			info.height = info.screen_width * 3 / 4;
            info.top = (info.screen_height - info.height) / 2;
		}
        else {
			info.width = info.screen_width;
			info.height = info.screen_height;
		}
        break;
    case 2: // force 16:9
        if (info.screen_width * 9 / 16 > info.screen_height) {
			info.height = info.screen_height;
			info.width = info.screen_height * 16 / 9;
            info.left = (info.screen_width - info.width) / 2;
		}
        else if (info.screen_height * 16 / 9 > info.screen_width) {
			info.width = info.screen_width;
			info.height = info.screen_width * 9 / 16;
            info.top = (info.screen_height - info.height) / 2;
		}
        else {
			info.width = info.screen_width;
			info.height = info.screen_height;
		}
        break;
    }

    return info;
}

void App::UpdateVideoOutputSize()
{
    if (m_emu.core.GetEmuState() != M64EMU_RUNNING)
        return;

    auto ws = ImGui::GetMainViewport()->GetWorkSize();
    auto wsw = static_cast<int>(ws.x) / 4 * 4;
    auto wsh = static_cast<int>(ws.y);
    auto size = m_emu.core.GetVideoSize();

	if (static_cast<int>(size.uiWidth) != wsw || static_cast<int>(size.uiHeight) != wsh)
        m_emu.core.GfxResizeOutput(static_cast<int>(wsw), static_cast<int>(wsh));
}

void App::ToggleFullScreen()
{
    m_video.window.ToggleFullScreen();
    m_emu.core.SetVideoMode(m_video.window.IsFullScreen() ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED);
}

void App::CaptureFrame()
{
    if (m_emu.core.GetEmuState() == M64EMU_STOPPED)
        return;

    auto size = m_emu.core.GetVideoSize();
    m_capture.width = static_cast<int>(size.uiWidth);
    m_capture.height = static_cast<int>(size.uiHeight);

    m_capture.pixels.resize(m_capture.width * m_capture.height * 3);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, m_capture.width, m_capture.height, GL_RGB, GL_UNSIGNED_BYTE, m_capture.pixels.data());

    glBindTexture(GL_TEXTURE_2D, m_capture.texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_capture.width, m_capture.height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, m_capture.pixels.data());
}

void App::DispatchCoreEvents()
{
    std::lock_guard lock{m_emu.events_mutex};

    if (!m_emu.events.empty()) {
        for (auto& e : m_emu.events)
            OnCoreEvent.fire(e & 0xff, (e >> 8) & 0xff);

        m_emu.events.clear();
    }
}

void App::DispatchCoreStates()
{
    std::lock_guard lock{m_emu.states_mutex};

    if (!m_emu.states.empty()) {
        for (const auto& p : m_emu.states)
            OnCoreStateChanged.fire(p.first, p.second);

        m_emu.states.clear();
    }
}

void App::DispatchEvents()
{
    DoEvents();

    DispatchCoreEvents();
    DispatchCoreStates();

    if (m_emu.notify_started) {
        OnCoreStarted.fire();
        m_emu.notify_started = false;
    }

    if (m_emu.notify_reset) {
        OnCoreReset.fire(m_emu.notify_reset == 2);
        m_emu.notify_reset = 0;
    }

    if (m_emu.stopping && m_emu.execute.wait_for(std::chrono::milliseconds{1}) == std::future_status::ready) {
        OnCoreStopped.fire();
        m_emu.stopping = false;
    }
}

void App::DestroyTextureLater(u32 id)
{
    std::lock_guard lock{m_tex_to_destroy_mutex};
    m_tex_to_destroy.push_back(id);
}

void App::DestroyTextures()
{
    std::lock_guard lock{m_tex_to_destroy_mutex};

    if (m_tex_to_destroy.empty())
        return;

    for (auto id : m_tex_to_destroy)
        Gfx::Texture::Destroy(id);

    m_tex_to_destroy.clear();
}

void App::BindingBeforeCreateResources()
{
    m_video.gl_context.MakeCurrent(m_video.window);
}

void App::BindingAfterCreateResources()
{
    SDL::GLContext::MakeCurrentNone();
}

void App::BindingBeforeRender()
{
    m_video.gl_context.MakeCurrent(m_video.window);

    if (m_emu.core.GetEmuState() == M64EMU_PAUSED) {
        glScissor(0, 0, m_video.window.GetWidth(), m_video.window.GetHeight());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    if (m_fonts.rebuild_atlas) {
        m_video.imgui.RebuildFontAtlas();
        m_fonts.rebuild_atlas = false;
    }

    m_video.imgui.NewFrame(m_video.window);
    ImGui::PushFont(m_fonts.base);

    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetWorkPos(), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->GetWorkSize());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.01f, 0.01f});
    ImGui::Begin("##main_overlay", nullptr,  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking);
    ImGui::PopStyleVar();
}

void App::BindingAfterRender()
{
    ShowUtilWindows();
    m_any_item_active = ImGui::IsAnyItemActive();
    ImGui::End();

    ImGui::PopFont();
    m_video.imgui.EndFrame();

    m_video.gl_context.MakeCurrent(m_video.window);
    m_video.window.Swap();
    SDL::GLContext::MakeCurrentNone();
}

void App::CoreStartedHandler()
{
    m_emu.elapsed_frames = 0;
    m_emu.gfx_aspect = m_emu.core.ConfigOpenSection("Video-GLideN64").GetIntOr("AspectRatio", 1);

    LoadROMCheats();
    InitUtilWindows();

    m_video.window.Show();
    m_video.window.Raise();
}

void App::CoreStoppedHandler()
{
    m_video.window.Hide();
    DeinitUtilWindows();
}

void App::CreateResourcesHandler()
{
    auto bound_tex = Gfx::Texture::GetCurrentBound();
    SDL::GLContext::MakeCurrentNone();

    OnCreateResources.fire();

    m_video.gl_context.MakeCurrent(m_video.window);
    glBindTexture(GL_TEXTURE_2D, bound_tex > 0 ? bound_tex : 0);
}

void App::NewFrameHandler()
{
    ++m_emu.elapsed_frames;
    OnNewFrame.fire();
}

void App::NewVIHandler()
{
    DestroyTextures();

    if (m_create_res_next) {
        CreateResourcesHandler();
        m_create_res_next = false;
    }

    if (m_take_shot) {
        m_take_shot = false;

        try {
            TakeScreenshot();
        }
        catch (const std::exception& e) {
            Logger::Log(LogCategory::Error, "M64p frontend", fmt::format("Screenshot not taken. {}", e.what()));
        }
    }

    m_input.Update(m_emu.core);
    OnNewVI.fire();
}

void App::ResetHandler(bool do_hard_reset)
{
    m_emu.notify_reset = 1 + do_hard_reset;
}

void App::PauseLoopHandler()
{
    auto ticks = SDL_GetTicks();

    NewVIHandler();
    SwapHandler();

    if (SDL_GetTicks() - ticks < 16)
        SDL_Delay(16 - (SDL_GetTicks() - ticks));
}

void App::CoreEventHandler(int event)
{
    if (!HasEmuInputFocus())
        return;

    std::lock_guard lock{m_emu.events_mutex};
    m_emu.events.push_back(event);
}

void App::CoreStateChangedHandler(m64p_core_param param_type, int new_value)
{
    if (!m_emu.started && param_type == M64CORE_EMU_STATE && new_value == M64EMU_RUNNING)
        m_emu.started = m_emu.notify_started = true;

    std::lock_guard lock{m_emu.states_mutex};
    m_emu.states.push_back(std::make_pair(param_type, new_value));
}

void App::DebugInitHandler()
{
    m_emu.debug_init = true;
    OnDebugInit.fire();
}

void App::DebugUpdateHandler(unsigned pc)
{
    OnDebugUpdate.fire(pc);
}

void App::SwapHandler()
{
    auto bound_tex = Gfx::Texture::GetCurrentBound();
    SDL::GLContext::MakeCurrentNone();

    OnViUpdate.fire();

    if (m_emu.core.GetEmuState() != M64EMU_PAUSED)
        m_video.gl_context.MakeCurrent(m_video.window);

    glBindTexture(GL_TEXTURE_2D, bound_tex > 0 ? bound_tex : 0);
    UpdateVideoOutputSize();
}

std::filesystem::path App::GetScreenshotPath()
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    m64p_rom_settings settings;
    m_emu.core.GetROMSettings(&settings);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now), "[%Y-%m-%d %H-%M-%S]") << " (" << m_emu.elapsed_frames << ") " << settings.goodname << ".png";
    auto filename = oss.str();

    auto cache_path = m_emu.core.GetUserCachePath();
    auto screenshot_path = std::filesystem::path{m_emu.core.ConfigOpenSection("Core").GetStringOr("ScreenshotPath", "")};

    return screenshot_path.empty() ? cache_path / "screenshot" / filename : screenshot_path / filename;
}

void App::TakeScreenshot()
{
    auto screenshot_path = GetScreenshotPath();
    auto parent_path = screenshot_path.parent_path();

    if (!std::filesystem::exists(parent_path))
        std::filesystem::create_directory(parent_path);

    auto width = m_video.window.GetWidth();
    auto height = m_video.window.GetHeight();
    const std::size_t pitch = width * 4;
    std::vector<u8> buf(width * height * 4);
    std::vector<u8> stride(pitch);

    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());

    for (int i{}; i < height / 2; ++i) {
        auto line_start = buf.data() + i * pitch;
        auto line_end = buf.data() + (height - 1 - i) * pitch;

        std::memcpy(stride.data(), line_start, pitch);
        std::memcpy(line_start, line_end, pitch);
        std::memcpy(line_end, stride.data(), pitch);
    }

    SDL::Surface s{buf.data(), width, height, 32, static_cast<int>(pitch), SDL_PIXELFORMAT_ABGR8888};
    s.SavePNG(GetScreenshotPath());
}

}

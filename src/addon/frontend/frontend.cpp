#include "addon/gfx/texture.h"
#include "addon/imgui/font_ref.h"
#include "addon/imgui/param.h"
#include "addon/sdl/window_ref.h"
#include "addon/param.h"
#include "addon/safe_call.h"
#include "common/logger.h"
#include "common/message_box.h"
#include "frontend/cheat_conf/view.h"
#include "frontend/input_conf/view.h"
#include "frontend/mem_viewer/view.h"
#include "frontend/app.h"

#include <array>
#include <atomic>
#include <unordered_map>

#include <fmt/format.h>

using namespace Param;

namespace Addon::Frontend {

enum CallbackIndex {
    CallbackIndex_CoreStarted,
    CallbackIndex_CoreStopped,
    CallbackIndex_NewFrame,
    CallbackIndex_NewVI,
    CallbackIndex_CoreReset,
    CallbackIndex_CoreEvent,
    CallbackIndex_CoreStateChanged,
    CallbackIndex_DebugInit,
    CallbackIndex_DebugUpdate,
    CallbackIndex_CreateResources,
    CallbackIndex_ViUpdate,
    CallbackIndex_WindowClosing,
    CallbackIndex_Count
};

static std::unordered_map<std::string, CallbackIndex> s_cb_map{
    std::make_pair("core-started", CallbackIndex_CoreStarted),
    {"core-stopped", CallbackIndex_CoreStopped},
    {"new-frame", CallbackIndex_NewFrame},
    {"new-vi", CallbackIndex_NewVI},
    {"core-reset", CallbackIndex_CoreReset},
    {"core-event", CallbackIndex_CoreEvent},
    {"core-state-changed", CallbackIndex_CoreStateChanged},
    {"debug-init", CallbackIndex_DebugInit},
    {"debug-update", CallbackIndex_DebugUpdate},
    {"create-resources", CallbackIndex_CreateResources},
    {"vi-update", CallbackIndex_ViUpdate},
    {"window-closing", CallbackIndex_WindowClosing}
};

static std::array<const char*, CallbackIndex_Count> s_cb_names{
    "core-started", "core-stopped", "new-frame", "new-vi", "core-reset",
    "core-event", "core-state-changed", "debug-init", "debug-update",
    "create-resources", "vi-update", "window-closing"
};

struct FuncRef {
    Napi::FunctionReference ref;
    Napi::ThreadSafeFunction ts;
};

static std::array<std::unique_ptr<FuncRef>, CallbackIndex_Count> s_cb_func;
static std::atomic_bool s_cb_lock;

inline ::Frontend::App& GetApp()
{
    return ::Frontend::App::GetInstance();
}

namespace CB {

template <class F>
void Call(int index, const F& f)
{
    auto& cb_func = s_cb_func[index];

    if (!cb_func || !cb_func->ref)
        return;

    cb_func->ts.BlockingCall([index, f](Napi::Env env, Napi::Function callback) {
        try {
            f(env, callback);
        }
        catch (const std::exception& e) {
            Logger::Log(LogCategory::Fatal, "M64p binding", fmt::format("Caught unhandled exception in callback '{}'. {}", s_cb_names[index], e.what()));
            std::exit(EXIT_FAILURE);
        }
        catch (...) {
            Logger::Log(LogCategory::Fatal, "M64p binding", fmt::format("Caught unhandled exception in callback '{}'", s_cb_names[index]));
            std::exit(EXIT_FAILURE);
        }
    });
}

template <class F>
void CallLock(int index, const F& f)
{
    auto& cb_func = s_cb_func[index];

    if (!cb_func || !cb_func->ref)
        return;

    s_cb_lock = false;

    cb_func->ts.BlockingCall([index, f](Napi::Env env, Napi::Function callback) {
        try {
            f(env, callback);
        }
        catch (const std::exception& e) {
            Logger::Log(LogCategory::Fatal, "M64p binding", fmt::format("Caught unhandled exception in callback '{}'. {}", s_cb_names[index], e.what()));
            std::exit(EXIT_FAILURE);
        }
        catch (...) {
            Logger::Log(LogCategory::Fatal, "M64p binding", fmt::format("Caught unhandled exception in callback '{}'", s_cb_names[index]));
            std::exit(EXIT_FAILURE);
        }

        s_cb_lock = true;
    });

    while (!s_cb_lock);
}

void CoreStartedHandler()
{
    Call(CallbackIndex_CoreStarted, [=](Napi::Env, Napi::Function callback)
    {
        callback.Call({});
    });
}

void CoreStoppedHandler()
{
    Call(CallbackIndex_CoreStopped, [=](Napi::Env, Napi::Function callback)
    {
        callback.Call({});
    });
}

void NewFrameHandler()
{
    CallLock(CallbackIndex_NewFrame, [](Napi::Env, Napi::Function callback)
    {
        callback.Call({});
    });
}

void NewVIHandler()
{
    CallLock(CallbackIndex_NewVI, [](Napi::Env, Napi::Function callback)
    {
        callback.Call({});
    });
}

void CoreResetHandler(bool do_hard_reset)
{
    Call(CallbackIndex_CoreReset, [=](Napi::Env env, Napi::Function callback)
    {
        callback.Call({FromBool(env, do_hard_reset)});
    });
}

void CoreEventHandler(int event, int data)
{
    Call(CallbackIndex_CoreEvent, [=](Napi::Env env, Napi::Function callback)
    {
        callback.Call({FromS32(env, event), FromS32(env, data)});
    });
}

void CoreStateChangedHandler(m64p_core_param param_type, int new_value)
{
    Call(CallbackIndex_CoreStateChanged, [=](Napi::Env env, Napi::Function callback)
    {
        callback.Call({FromS32(env, param_type), FromS32(env, new_value)});
    });
}

void DebugInitHandler()
{
    CallLock(CallbackIndex_DebugInit, [](Napi::Env, Napi::Function callback)
    {
        callback.Call({});
    });
}

void DebugUpdateHandler(unsigned pc)
{
    CallLock(CallbackIndex_DebugUpdate, [=](Napi::Env env, Napi::Function callback)
    {
        callback.Call({FromU32(env, pc)});
    });
}

void CreateResourcesHandler()
{
    CallLock(CallbackIndex_CreateResources, [=](Napi::Env, Napi::Function callback)
    {
        GetApp().BindingBeforeCreateResources();
        callback.Call({});
        GetApp().BindingAfterCreateResources();
    });
}

void ViUpdateHandler()
{
    if (s_cb_func[CallbackIndex_ViUpdate]) {
        CallLock(CallbackIndex_ViUpdate, [=](Napi::Env, Napi::Function callback)
        {
            GetApp().BindingBeforeRender();
            callback.Call({});
            GetApp().BindingAfterRender();
        });
    }
    else {
        GetApp().BindingBeforeRender();
        GetApp().BindingAfterRender();
    }
}

void WindowClosingHandler()
{
    Call(CallbackIndex_WindowClosing, [=](Napi::Env, Napi::Function callback)
    {
        callback.Call({});
    });
}

void Link()
{
    auto& app = GetApp();
    app.OnCoreStarted.connect<CoreStartedHandler>();
    app.OnCoreStopped.connect<CoreStoppedHandler>();
    app.OnNewFrame.connect<NewFrameHandler>();
    app.OnNewVI.connect<NewVIHandler>();
    app.OnCoreReset.connect<CoreResetHandler>();
    app.OnCoreEvent.connect<CoreEventHandler>();
    app.OnCoreStateChanged.connect<CoreStateChangedHandler>();
    app.OnDebugInit.connect<DebugInitHandler>();
    app.OnDebugUpdate.connect<DebugUpdateHandler>();
    app.OnCreateResources.connect<CreateResourcesHandler>();
    app.OnViUpdate.connect<ViUpdateHandler>();
    app.OnWindowClosing.connect<WindowClosingHandler>();
}

void Unlink()
{
    auto& app = GetApp();
    app.OnCoreStarted.disconnect<CoreStartedHandler>();
    app.OnCoreStopped.disconnect<CoreStoppedHandler>();
    app.OnNewFrame.disconnect<NewFrameHandler>();
    app.OnNewVI.disconnect<NewVIHandler>();
    app.OnCoreReset.disconnect<CoreResetHandler>();
    app.OnCoreEvent.disconnect<CoreEventHandler>();
    app.OnCoreStateChanged.disconnect<CoreStateChangedHandler>();
    app.OnDebugInit.disconnect<DebugInitHandler>();
    app.OnDebugUpdate.disconnect<DebugUpdateHandler>();
    app.OnCreateResources.disconnect<CreateResourcesHandler>();
    app.OnViUpdate.disconnect<ViUpdateHandler>();
    app.OnWindowClosing.disconnect<WindowClosingHandler>();
}

}

static Napi::ObjectReference s_frame_texture;

Napi::Value Startup(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        auto obj = AsObject(info[0]);
        std::vector<u8> icon_data;

        if (!obj.Get("windowIcon").IsUndefined()) {
            auto icon_buf = obj.Get("windowIcon").As<Napi::Uint8Array>();
            icon_data.resize(icon_buf.ByteLength());
            std::copy(icon_buf.Data(), icon_buf.Data() + icon_buf.ByteLength(), icon_data.begin());
        }

        ::Frontend::StartInfo start_info{
            AsStrUtf8(obj.Get("windowTitle")),
            AsS32(obj.Get("windowWidth")),
            AsS32(obj.Get("windowHeight")),
            icon_data,
            AsPath(obj.Get("corePlugin")),
            AsPath(obj.Get("rspPlugin")),
            AsPath(obj.Get("videoPlugin")),
            AsPath(obj.Get("audioPlugin")),
            AsPath(obj.Get("inputPlugin")),
            AsPath(obj.Get("configDir")),
            AsPath(obj.Get("dataDir"))
        };

        GetApp().Startup(start_info);
        CB::Link();

        auto frame_capture = GetApp().GetFrameCaptureInfo();
        s_frame_texture = Napi::Persistent(Gfx::Texture::Create(info.Env(),
            {frame_capture.texture_id, frame_capture.width, frame_capture.height}, false));

        return info.Env().Undefined();
    });
}

Napi::Value Shutdown(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        CB::Unlink();

        for (auto& cb_func : s_cb_func) {
            if (cb_func) {
                cb_func->ref.Unref();
                cb_func->ts.Abort();
                cb_func->ts.Unref(info.Env());
            }

            cb_func.reset();
        }

        GetApp().Shutdown();

        return info.Env().Undefined();
    });
}

Napi::Value DoEvents(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetApp().DispatchEvents();

        return info.Env().Undefined();
    });
}

Napi::Value Execute(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetApp().Execute();

        return info.Env().Undefined();
    });
}

Napi::Value Stop(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetApp().Stop();

        return info.Env().Undefined();
    });
}

Napi::Value On(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        auto name = AsStrUtf8(info[0]);
        auto index = s_cb_map.at(name);
        auto func = info[1].As<Napi::Function>();
        auto& cb_func = s_cb_func[index];

        if (cb_func) {
            Logger::Log(LogCategory::Warn, "M64p binding", fmt::format("Overriding callback '{}'. `{}` -> `{}`", s_cb_names[index],
                AsStrUtf8(cb_func->ref.Value().ToString()), AsStrUtf8(func.ToString())));

            cb_func->ref.Unref();
            cb_func->ts.Abort();
            cb_func->ts.Unref(info.Env());
        }

        cb_func = std::make_unique<FuncRef>();
        cb_func->ref = Napi::Persistent(func);
        cb_func->ts = Napi::ThreadSafeFunction::New(info.Env(), cb_func->ref.Value(),
            std::string{"front_cb_" + name}, 0, 1);

        return info.Env().Undefined();
    });
}

Napi::Value GetVideoOuputInfo(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        const auto out_info = GetApp().GetVideoOutputInfo();

		auto ret = Napi::Object::New(info.Env());
		ret.Set("screenWidth", FromF32(info.Env(), out_info.screen_width));
		ret.Set("screenHeight", FromF32(info.Env(), out_info.screen_height));
		ret.Set("left", FromF32(info.Env(), out_info.left));
		ret.Set("top", FromF32(info.Env(), out_info.top));
		ret.Set("width", FromF32(info.Env(), out_info.width));
		ret.Set("height", FromF32(info.Env(), out_info.height));

		return ret;
    });
}

Napi::Value ToggleFullScreen(const Napi::CallbackInfo& info)
{
    GetApp().ToggleFullScreen();

    return info.Env().Undefined();
}

Napi::Value CaptureFrame(const Napi::CallbackInfo& info)
{
    return SafeCall(info.Env(), [&info]() {
        GetApp().CaptureFrame();

        auto frame_capture = GetApp().GetFrameCaptureInfo();
        Gfx::Texture::Unwrap(s_frame_texture.Value())->m_texture.UpdateSize(frame_capture.width, frame_capture.height);

        return info.Env().Undefined();
    });
}

Napi::Value GetFrameTexture(const Napi::CallbackInfo&)
{
    return s_frame_texture.Value();
}

Napi::Value CreateResourcesNextVi(const Napi::CallbackInfo& info)
{
    GetApp().CreateResourcesNextVi();

    return info.Env().Undefined();
}

Napi::Value TakeNextScreenshot(const Napi::CallbackInfo& info)
{
    GetApp().TakeNextScreenshot();

    return info.Env().Undefined();
}

Napi::Value GetMainWindow(const Napi::CallbackInfo& info)
{
    return SDL::WindowRef::Create(info.Env(), GetApp().GetMainWindow().Get());
}

Napi::Value ShowMessageBox(const Napi::CallbackInfo& info)
{
    SDL_Window* parent = info[0].IsUndefined() ? nullptr : SDL::WindowRef::Unwrap(AsObject(info[0]))->m_window;
    MessageBoxShow(parent, static_cast<MessageBoxIcon>(AsS32(info[1])), AsStrUtf8(info[2]), AsStrUtf8(info[3]), AsStrUtf8(info[4]));

    return info.Env().Undefined();
}

Napi::Value GetDefaultFont(const Napi::CallbackInfo& info)
{
    return ImGui_::FontRef::Create(info.Env(), GetApp().GetDefaultFont());
}

Napi::Value SetDefaultFont(const Napi::CallbackInfo& info)
{
    GetApp().SetDefaultFont(ImGui_::Param::AsFontPtr(info[0]));

    return info.Env().Undefined();
}

Napi::Value GetDefaultMonoFont(const Napi::CallbackInfo& info)
{
    return ImGui_::FontRef::Create(info.Env(), GetApp().GetDefaultMonoFont());
}

Napi::Value SetDefaultMonoFont(const Napi::CallbackInfo& info)
{
    GetApp().SetDefaultMonoFont(ImGui_::Param::AsFontPtr(info[0]));

    return info.Env().Undefined();
}

Napi::Value OpenInputConfig(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetInputConf();

    if (v)
        v->Open();

    return info.Env().Undefined();
}

Napi::Value CloseInputConfig(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetInputConf();

    if (v)
        v->Close();

    return info.Env().Undefined();
}

Napi::Value IsInputConfigOpen(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetInputConf();

    return FromBool(info.Env(), v ? v->IsOpen() : false);
}

Napi::Value OpenCheatConfig(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetCheatConf();

    if (v)
        v->Open();

    return info.Env().Undefined();
}

Napi::Value CloseCheatConfig(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetCheatConf();

    if (v)
        v->Close();

    return info.Env().Undefined();
}

Napi::Value IsCheatConfigOpen(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetCheatConf();

    return FromBool(info.Env(), v ? v->IsOpen() : false);
}

Napi::Value SetCheatsCRC(const Napi::CallbackInfo& info)
{
    GetApp().OverrideCheatsCrc(AsStrUtf8(info[0]));

    return info.Env().Undefined();
}

Napi::Value DisableAllCheats(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetCheatConf();

    if (v)
        v->DisableAllEntries();

    return info.Env().Undefined();
}

Napi::Value OpenMemViewer(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetMemViewer();

    if (v)
        v->Open();

    return info.Env().Undefined();
}

Napi::Value CloseMemViewer(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetMemViewer();

    if (v)
        v->Close();

    return info.Env().Undefined();
}

Napi::Value IsMemViewerOpen(const Napi::CallbackInfo& info)
{
    auto v = GetApp().GetMemViewer();

    return FromBool(info.Env(), v ? v->IsOpen() : false);
}

Napi::Object BuildExports(Napi::Env env, Napi::Object exports)
{
    exports.Set("startup", Napi::Function::New(env, Startup));
    exports.Set("shutdown", Napi::Function::New(env, Shutdown));
    exports.Set("doEvents", Napi::Function::New(env, DoEvents));
    exports.Set("execute", Napi::Function::New(env, Execute));
    exports.Set("stop", Napi::Function::New(env, Stop));
    exports.Set("on", Napi::Function::New(env, On));

    exports.Set("getVideoOutputInfo", Napi::Function::New(env, GetVideoOuputInfo));
    exports.Set("toggleFullScreen", Napi::Function::New(env, ToggleFullScreen));
    exports.Set("captureFrame", Napi::Function::New(env, CaptureFrame));
    exports.Set("getFrameTexture", Napi::Function::New(env, GetFrameTexture));
    exports.Set("createResourcesNextVi", Napi::Function::New(env, CreateResourcesNextVi));
    exports.Set("takeNextScreenshot", Napi::Function::New(env, TakeNextScreenshot));
    exports.Set("getMainWindow", Napi::Function::New(env, GetMainWindow));
    exports.Set("showMessageBox", Napi::Function::New(env, ShowMessageBox));

    exports.Set("getDefaultFont", Napi::Function::New(env, GetDefaultFont));
    exports.Set("setDefaultFont", Napi::Function::New(env, SetDefaultFont));
    exports.Set("getDefaultMonoFont", Napi::Function::New(env, GetDefaultMonoFont));
    exports.Set("setDefaultMonoFont", Napi::Function::New(env, SetDefaultMonoFont));

    exports.Set("openInputConfig", Napi::Function::New(env, OpenInputConfig));
    exports.Set("closeInputConfig", Napi::Function::New(env, CloseInputConfig));
    exports.Set("isInputConfigOpen", Napi::Function::New(env, IsInputConfigOpen));
    exports.Set("openCheatConfig", Napi::Function::New(env, OpenCheatConfig));
    exports.Set("closeCheatConfig", Napi::Function::New(env, CloseCheatConfig));
    exports.Set("isCheatConfigOpen", Napi::Function::New(env, IsCheatConfigOpen));
    exports.Set("setCheatsCRC", Napi::Function::New(env, SetCheatsCRC));
    exports.Set("disableAllCheats", Napi::Function::New(env, DisableAllCheats));
    exports.Set("openMemViewer", Napi::Function::New(env, OpenMemViewer));
    exports.Set("closeMemViewer", Napi::Function::New(env, CloseMemViewer));
    exports.Set("isMemViewerOpen", Napi::Function::New(env, IsMemViewerOpen));

    return exports;
}

}

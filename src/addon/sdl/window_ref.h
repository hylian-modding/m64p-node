#pragma once

#include <napi.h>

struct SDL_Window;

namespace Addon::SDL {

struct WindowRef : Napi::ObjectWrap<WindowRef> {
    static Napi::FunctionReference constructor;
	SDL_Window* m_window;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object Create(Napi::Env env, SDL_Window* window);

    WindowRef(const Napi::CallbackInfo& info);

    Napi::Value GetFlags(const Napi::CallbackInfo& info);
    Napi::Value GetMaximumSize(const Napi::CallbackInfo& info);
    Napi::Value GetMinimumSize(const Napi::CallbackInfo& info);
    Napi::Value GetOpacity(const Napi::CallbackInfo& info);
    Napi::Value GetPosition(const Napi::CallbackInfo& info);
    Napi::Value GetSize(const Napi::CallbackInfo& info);
    Napi::Value GetTitle(const Napi::CallbackInfo& info);
    Napi::Value Hide(const Napi::CallbackInfo& info);
    Napi::Value Maximize(const Napi::CallbackInfo& info);
    Napi::Value Minimize(const Napi::CallbackInfo& info);
    Napi::Value Raise(const Napi::CallbackInfo& info);
    Napi::Value Restore(const Napi::CallbackInfo& info);
    Napi::Value SetBordered(const Napi::CallbackInfo& info);
    Napi::Value SetFullScreen(const Napi::CallbackInfo& info);
    Napi::Value SetInputFocus(const Napi::CallbackInfo& info);
    Napi::Value SetMaximumSize(const Napi::CallbackInfo& info);
    Napi::Value SetMinimumSize(const Napi::CallbackInfo& info);
    Napi::Value SetOpacity(const Napi::CallbackInfo& info);
    Napi::Value SetPosition(const Napi::CallbackInfo& info);
    Napi::Value SetResizable(const Napi::CallbackInfo& info);
    Napi::Value SetSize(const Napi::CallbackInfo& info);
    Napi::Value SetTitle(const Napi::CallbackInfo& info);
    Napi::Value Show(const Napi::CallbackInfo& info);
    Napi::Value EqualsTo(const Napi::CallbackInfo& info);
};

}

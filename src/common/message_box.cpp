#include "common/message_box.h"

#include <stdexcept>

#include <fmt/format.h>
#include <SDL2/SDL_messagebox.h>
#include <SDL2/SDL_platform.h>
#include <SDL2/SDL_syswm.h>

#ifdef __WINDOWS__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shobjidl.h>
#include <atlstr.h>
#include <CommCtrl.h>
#undef MessageBox

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "Comctl32")

#elif defined(__LINUX__)

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#endif

#ifdef __WINDOWS__

static HWND GetHandle(SDL_Window* window)
{
    if (!window)
        return nullptr;

    SDL_SysWMinfo wm_info;
    SDL_VERSION(&wm_info.version);
    SDL_GetWindowWMInfo(window, &wm_info);

    return wm_info.info.win.window;
}

static std::wstring CStrToWStr(const std::string& str)
{
    if (str.empty())
        return{};

    int size = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, str.c_str(), static_cast<int>(str.size()), nullptr, 0);

    if (size == 0)
        throw std::runtime_error{"Failed to convert string to wide string"};

    std::wstring wstr(size, '\0');
    size = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, str.c_str(), static_cast<int>(str.size()), wstr.data(), static_cast<int>(wstr.size()));

    if (size == 0)
        throw std::runtime_error{"Failed to convert string to wide string"};

    return wstr;
}

void MessageBoxShow(SDL_Window* parent, MessageBoxIcon icon, const std::string& title,
    const std::string& inst, const std::string& content)
{
    PCWSTR real_icon{};

    switch (icon) {
    case MessageBoxIcon::Error:
        real_icon = TD_ERROR_ICON;
        break;
    case MessageBoxIcon::Warning:
        real_icon = TD_WARNING_ICON;
        break;
    case MessageBoxIcon::Information:
    default:
        real_icon = TD_INFORMATION_ICON;
    }

    TaskDialog(
        GetHandle(parent),
        nullptr,
        CStrToWStr(title).c_str(),
        CStrToWStr(inst).c_str(),
        CStrToWStr(content).c_str(),
        TDCBF_OK_BUTTON,
        real_icon,
        nullptr);
}

#elif defined(__LINUX__)

void MessageBoxShow(SDL_Window* parent, MessageBoxIcon icon, const std::string& title,
    const std::string& inst, const std::string& content)
{
    if (!gtk_init_check(0, nullptr))
        throw std::runtime_error{"Gtk not initialized"};

    GtkMessageType real_icon{};

    switch (icon) {
    case MessageBoxIcon::Error:
        real_icon = GTK_MESSAGE_ERROR;
        break;
    case MessageBoxIcon::Warning:
        real_icon = GTK_MESSAGE_WARNING;
        break;
    case MessageBoxIcon::Information:
    default:
        real_icon = GTK_MESSAGE_INFO;
        break;
    case MessageBoxIcon::Question:
        real_icon = GTK_MESSAGE_QUESTION;
    }

    GtkWidget* dialog;

    if (!inst.empty())
        dialog = gtk_message_dialog_new(nullptr, GTK_DIALOG_MODAL, real_icon, GTK_BUTTONS_OK,
            "%s\n\n%s", inst.c_str(), content.c_str());
    else
        dialog = gtk_message_dialog_new(nullptr, GTK_DIALOG_MODAL, real_icon, GTK_BUTTONS_OK,
            "%s", content.c_str());

    if (parent) {
        SDL_SysWMinfo wm_info;
        SDL_VERSION(&wm_info.version);
        SDL_GetWindowWMInfo(parent, &wm_info);

#ifdef GDK_WINDOWING_X11
        auto foreign_window = gdk_x11_window_foreign_new_for_display(gtk_widget_get_display(dialog), wm_info.info.x11.window);
        gtk_widget_realize(dialog);
        auto dialog_window = gtk_widget_get_window(dialog);
        gdk_window_set_transient_for(dialog_window, foreign_window);
#endif
    }

    gtk_window_set_title(GTK_WINDOW(dialog), title.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(GTK_WIDGET(dialog));
    while (g_main_context_iteration(nullptr, false));
}

#else

void MessageBoxShow(SDL_Window* parent, MessageBoxIcon icon, const std::string& title,
    const std::string& inst, const std::string& content)
{
    SDL_MessageBoxFlags real_icon{};

    switch (icon) {
    case MessageBoxIcon::Error:
        real_icon = SDL_MESSAGEBOX_ERROR;
        break;
    case MessageBoxIcon::Warning:
        real_icon = SDL_MESSAGEBOX_WARNING;
        break;
    case MessageBoxIcon::Information:
    case MessageBoxIcon::Question:
    default:
        real_icon = SDL_MESSAGEBOX_INFORMATION;
    }

    if (!inst.empty())
        SDL_ShowSimpleMessageBox(real_icon, title.c_str(), fmt::format("{}\n\n{}", inst, content).c_str(), parent);
    else
        SDL_ShowSimpleMessageBox(real_icon, title.c_str(), fmt::format("{}", content).c_str(), parent);
}

#endif

#include "frontend/imgui_ctx.h"
#include "imgui/imgui.h"
#include "imgui/imgui_freetype.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "sdl/gl_context.h"
#include "sdl/window.h"

#include <stdexcept>

namespace Frontend {

ImGuiCtx::~ImGuiCtx()
{
    if (m_init)
        Deinitialize();
}

void ImGuiCtx::InitContext()
{
    m_ctx = ImGui::CreateContext();

    if (!m_ctx)
        throw std::runtime_error{"ImGuiCtx: Context initialization failed"};

    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable;
}

void ImGuiCtx::DeinitContext()
{
    ImGui::DestroyContext();
}

void ImGuiCtx::InitBackends(SDL::Window& window, SDL::GLContext& gl_context)
{
    if (!ImGui_ImplSDL2_InitForOpenGL(window.Get(), gl_context.Get()))
        throw std::runtime_error{"ImGuiCtx: SDL2 backend initialization failed"};

    if (!ImGui_ImplOpenGL3_Init("#version 330"))
        throw std::runtime_error{"ImGuiCtx: OpenGL3 backend initialization failed"};
}

void ImGuiCtx::DeinitBackends()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
}

void ImGuiCtx::Initialize(SDL::Window& window, SDL::GLContext& gl_context)
{
    if (m_init)
        Deinitialize();

    m_init = false;

    InitContext();
    InitBackends(window, gl_context);
    ApplyStyle();

    m_init = true;
}

void ImGuiCtx::Deinitialize()
{
    if (!m_init)
        return;

    DeinitBackends();
    DeinitContext();

    m_init = false;
}

bool ImGuiCtx::ProcessEvent(const SDL_Event& e)
{
    return ImGui_ImplSDL2_ProcessEvent(&e);
}

void ImGuiCtx::NewFrame(SDL::Window& window)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window.Get());
    ImGui::NewFrame();
}

void ImGuiCtx::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::EndFrame();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void ImGuiCtx::RebuildFontAtlas()
{
    ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts);

    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

void ImGuiCtx::ApplyStyle()
{
    ImGui::StyleColorsDark();

	auto& style = ImGui::GetStyle();
    auto& colors = ImGui::GetStyle().Colors;

    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
    style.ScrollbarSize = 16.0f;
    style.ItemSpacing.x = 10.0f;
    style.ItemSpacing.y = 4.0f;
    style.IndentSpacing = 22.0f;
    style.FramePadding.x = 6.0f;
    style.FramePadding.y = 4.0f;
    style.Alpha = 1.0f;
	style.FrameBorderSize = 0.0f;
	style.WindowBorderSize = 0.0f;
	style.PopupBorderSize = 0.0f;
	style.PopupRounding = 2.0f;
	style.TabBorderSize = 1.0f;
	style.WindowRounding = 1.5f;
	style.ChildRounding = 2.0f;
	style.ChildBorderSize = 0.0f;
	style.FrameRounding = 3.0f;
	style.ScrollbarRounding = 3.0f;
	style.GrabRounding = 1.0f;
	style.TabRounding = 5.0f;

    colors[ImGuiCol_WindowBg] = {0.141f, 0.141f, 0.141f, 0.940f};
    colors[ImGuiCol_CheckMark] = {0.254f, 0.665f, 0.307f, 1.000f};
    colors[ImGuiCol_MenuBarBg] = {0.072f, 0.072f, 0.072f, 1.000f};
}

}

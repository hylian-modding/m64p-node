
#include "frontend/input_conf/view.h"
#include "imgui/imgui.h"

#include <fmt/format.h>

namespace InputConf {

View::View() :
    m_cont_tabs{ContTab(0), ContTab(1), ContTab(2), ContTab(3)}
{}

void View::LoadConfig(M64P::Core& core)
{
    for (auto& cont : m_cont_tabs)
        cont.LoadConfig(core);
    m_events_tab.LoadConfig(core);
}

void View::SaveConfig(M64P::Core& core)
{
    for (auto& cont : m_cont_tabs)
        cont.SaveConfig(core);
    m_events_tab.SaveConfig(core);
}

void View::DoEvent(const SDL_Event& e)
{
    for (std::size_t i = 0; i < 4; ++i)
        m_cont_tabs[i].DoEvent(e, m_tab_index == i && IsOpen());

    if (m_tab_index == 4 && IsOpen())
        m_events_tab.DoEvent(e);
}

void View::Show(SDL::Window& main_win)
{
    if (!m_open)
        return;

    ImGui::SetNextWindowSize({380.0f, 816.0f}, ImGuiCond_Appearing);
    ImGui::Begin("Input config", &m_open, GetWindowFlags(main_win) | ImGuiWindowFlags_NoDocking);

    CommonBehavior();

    if (ImGui::BeginTabBar("##tabs")) {
        for (std::size_t i{}; i < 4; ++i) {
            if (ImGui::BeginTabItem(fmt::format("Cont. {}", i + 1).c_str())) {
                ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);

                if (ImGui::BeginChild("##cont", {}, true)) {
                    m_tab_index = i;
                    m_cont_tabs[i].Show();
                }
                ImGui::EndChild();

                ImGui::PopStyleVar();
                ImGui::EndTabItem();
            }
        }

        if (ImGui::BeginTabItem("Events")) {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);

            if (ImGui::BeginChild("##cont", {}, true)) {
                m_tab_index = 4;
                m_events_tab.Show();
            }
            ImGui::EndChild();

            ImGui::PopStyleVar();
            ImGui::EndTabItem();
        }
    }

    ImGui::EndTabBar();

    ImGui::End();
}

}

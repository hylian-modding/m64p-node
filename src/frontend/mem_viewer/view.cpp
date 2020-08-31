#include "frontend/mem_viewer/view.h"
#include "imgui/imgui.h"
#include "imgui/imgui_memory_editor.h"
#include "m64p/core.h"

namespace MemViewer {

View::View(M64P::Core& core) :
    m_core{&core}
{}

void View::Show(SDL::Window& main_win, ImFont* font_mono)
{
    if (!m_open)
       return;

    static MemoryEditor dram_edit;
    static MemoryEditor rom_edit;

    ImGui::SetNextWindowSize({720.0, 500.0f}, ImGuiCond_Appearing);
    ImGui::Begin("Memory viewer", &m_open, GetWindowFlags(main_win) | ImGuiWindowFlags_NoDocking);

    CommonBehavior();

    if (ImGui::BeginTabBar("tabs")) {
        if (ImGui::BeginTabItem("DRAM")) {
            ImGui::PushFont(font_mono);
            dram_edit.DrawContents(m_core->GetDRAMPtr(), m_core->GetDRAMSize());
            ImGui::PopFont();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("ROM")) {
            ImGui::PushFont(font_mono);
            rom_edit.DrawContents(m_core->GetROMPtr(), m_core->GetROMSize());
            ImGui::PopFont();
            ImGui::EndTabItem();
        }
    }

    ImGui::EndTabBar();
    ImGui::End();
}

}

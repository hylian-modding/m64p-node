#include "frontend/mem_viewer/view.h"
#include "imgui/imgui.h"
#include "imgui/imgui_memory_editor.h"
#include "m64p/core.h"

namespace MemViewer {

View::View(M64P::Core& core) :
    m_core{&core}
{}

static bool s_EditInit = false;

inline std::size_t GetByteOffset(size_t offset)
{
    return offset / 4 * 4 + (3 - (offset & 3));
}

u8 ReadFn(const u8* data, size_t offset)
{
    return data[GetByteOffset(offset)];
}

void WriteFn(u8* data, size_t offset, u8 value)
{
    data[GetByteOffset(offset)] = value;
}

void View::Show(SDL::Window& main_win, ImFont* font_mono)
{
    if (!m_open)
       return;

    static MemoryEditor dram_edit;
    static MemoryEditor rom_edit;

    if (!s_EditInit) {
        s_EditInit = true;
        dram_edit.ReadFn = rom_edit.ReadFn = ReadFn;
        dram_edit.WriteFn = rom_edit.WriteFn = WriteFn;
    }

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

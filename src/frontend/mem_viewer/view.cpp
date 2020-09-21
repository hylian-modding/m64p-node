#include "frontend/mem_viewer/view.h"
#include "imgui/imgui.h"
#include "imgui/imgui_memory_editor.h"
#include "m64p/core.h"

namespace MemViewer {

inline std::size_t GetByteOffset(size_t offset)
{
    return offset / 4 * 4 + (3 - (offset & 3));
}

static u8 ReadLEFn(const u8* data, size_t offset)
{
    return data[GetByteOffset(offset)];
}

static void WriteLEFn(u8* data, size_t offset, u8 value)
{
    data[GetByteOffset(offset)] = value;
}

class MemViewerImpl {
public:
    MemViewerImpl(M64P::Core& core) :
        m_core{&core}
    {
        m_dram_edit.ReadFn = m_rom_edit.ReadFn = ReadLEFn;
        m_dram_edit.WriteFn = m_rom_edit.WriteFn = WriteLEFn;
    }

    void DrawDRAMContents()
    {
        m_dram_edit.DrawContents(m_core->GetDRAMPtr(), m_core->GetDRAMSize());
    }

    void DrawROMContents()
    {
        m_rom_edit.DrawContents(m_core->GetROMPtr(), m_core->GetROMSize());
    }

    void SetLE()
    {
        m_dram_edit.ReadFn = m_rom_edit.ReadFn = ReadLEFn;
        m_dram_edit.WriteFn = m_rom_edit.WriteFn = WriteLEFn;
    }

    void SetBE()
    {
        m_dram_edit.ReadFn = m_rom_edit.ReadFn = nullptr;
        m_dram_edit.WriteFn = m_rom_edit.WriteFn = nullptr;
    }

    bool IsLE() const
    {
        return m_dram_edit.ReadFn == ReadLEFn;
    }

private:
    M64P::Core* m_core;
    MemoryEditor m_dram_edit;
    MemoryEditor m_rom_edit;
};

View::View(M64P::Core& core) :
    m_impl{std::make_unique<MemViewerImpl>(core)}
{
    m_impl->SetLE();
}

View::~View() = default;

void View::Show(SDL::Window& main_win, ImFont* font_mono)
{
    if (!m_open)
        return;

    ImGui::SetNextWindowSize({720.0, 500.0f}, ImGuiCond_Appearing);
    ImGui::Begin("Memory viewer", &m_open, GetWindowFlags(main_win) | ImGuiWindowFlags_NoDocking);

    CommonBehavior();

    bool is_le = m_impl->IsLE();
    ImGui::SetNextItemWidth(60.0f);

    if (ImGui::BeginCombo("##endian", is_le ? "LE" : "BE")) {
        if (ImGui::Selectable("LE", is_le))
            m_impl->SetLE();
        if (ImGui::Selectable("BE", !is_le))
            m_impl->SetBE();
        ImGui::EndCombo();
    }
    ImGui::SameLine();

    if (ImGui::BeginTabBar("tabs")) {
        if (ImGui::BeginTabItem("RDRAM")) {
            ImGui::PushFont(font_mono);
            m_impl->DrawDRAMContents();
            ImGui::PopFont();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("ROM")) {
            ImGui::PushFont(font_mono);
            m_impl->DrawROMContents();
            ImGui::PopFont();
            ImGui::EndTabItem();
        }
    }

    ImGui::EndTabBar();
    ImGui::End();
}

}

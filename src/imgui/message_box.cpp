#include "common/message_box.h"

void ImShowAssertMessageBox(bool ok, const char* error_msg)
{
    if (!ok) {
        MessageBoxShow(nullptr, MessageBoxIcon::Error, "ImGui Assertion Failed", "Assertion failed", error_msg);
        std::exit(EXIT_FAILURE);
    }
}

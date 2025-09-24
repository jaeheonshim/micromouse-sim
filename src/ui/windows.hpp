#pragma once

#include "imgui.h"
#include "core/mouse.hpp"

inline void DrawMouseWindow(const Mouse& mouse) {
    ImGui::Begin("Mouse");

    ImGui::Text("Pose x=%.3f, y=%.3f, heading=%.2f", mouse.get_pos_x(), mouse.get_pos_y(), mouse.get_heading());

    ImGui::End();
}
#pragma once

#include "imgui.h"
#include "core/mouse.hpp"

inline void DrawMouseWindow(Mouse& mouse) {
    ImGui::Begin("Mouse");

    ImGui::Text("Pose x=%.3f, y=%.3f, heading=%.2f", mouse.get_pos_x(), mouse.get_pos_y(), mouse.get_heading());
    ImGui::Text("Raycast x=%.3f, y=%.3f", mouse.sensorReadings[0].x, mouse.sensorReadings[0].y);
    ImGui::SeparatorText("Parameters");
    ImGui::Checkbox("Show Raycast", &mouse.showRaycast);
    double step{ 0.01 }; // centimeter
    ImGui::InputScalar("Width (m)", ImGuiDataType_Double, &mouse.width, &step, nullptr, "%.3f");
    ImGui::InputScalar("Length (m)", ImGuiDataType_Double, &mouse.length, &step, nullptr, "%.3f");
    ImGui::InputScalar("Wheel Radius (m)", ImGuiDataType_Double, &mouse.wheel_radius, &step, nullptr, "%.3f");

    ImGui::End();
}
#pragma once

#include <iostream>
#include "imgui.h"

struct ManualControlConfig {
    bool enabled{ false };

    ImGuiKey left_inc{ ImGuiKey_W };
    ImGuiKey left_dec{ ImGuiKey_S };
    ImGuiKey right_inc{ ImGuiKey_I };
    ImGuiKey right_dec{ ImGuiKey_K };
    ImGuiKey stop_all{ ImGuiKey_Space };

    double max_vel{ 30 };
    double vel_ramp{ 60 };
    double vel_decay{ 60 };
};

struct ManualWheelDriveAccum {
    double left{ 0.0f };
    double right{ 0.0f };
};

inline const char* KeyName(ImGuiKey k) {
    return ImGui::GetKeyName(k);
}

inline void KeyBindButton(const char* label, ImGuiKey& key_to_edit, bool& capturing) {
    ImGui::PushID(label);
    if (!capturing) {
        capturing = ImGui::Button(KeyName(key_to_edit));
    } else {
        ImGui::Button("[press any key]");

        if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
            capturing = false;
        } else {
            for (int k{ ImGuiKey_NamedKey_BEGIN }; k < ImGuiKey_NamedKey_END; ++k) {
                ImGuiKey kk{ (ImGuiKey)k };
                if (ImGui::IsKeyPressed(kk)) {
                    key_to_edit = kk;
                    capturing = false;
                    break;
                }
            }
        }
    }
    ImGui::PopID();
}

inline void GetWheelCommandsFromKeys(ManualWheelDriveAccum& acc, const ManualControlConfig& config, float dt) {
    if(!config.enabled) {
        acc.left = acc.right = 0;
        return;
    }

    ImGuiIO& io{ ImGui::GetIO() };

    double ramp{ config.vel_ramp * dt };
    double decay{ config.vel_decay * dt };

    if(ImGui::IsKeyDown(config.left_inc)) acc.left += ramp;
    if(ImGui::IsKeyDown(config.left_dec)) acc.left -= ramp;
    if(ImGui::IsKeyDown(config.right_inc)) acc.right += ramp;
    if(ImGui::IsKeyDown(config.right_dec)) acc.right -= ramp;

    auto decay0 = [&](double& v){
        if (v > 0.0) v = std::max(0.0, v - decay);
        else if (v < 0.0) v = std::min(0.0, v + decay);
    };

    if(!ImGui::IsKeyDown(config.left_inc) && ! ImGui::IsKeyDown(config.left_dec)) {
        decay0(acc.left);
    }

    if(!ImGui::IsKeyDown(config.right_inc) && ! ImGui::IsKeyDown(config.right_dec)) {
        decay0(acc.right);
    }

    acc.left = std::clamp(acc.left, -config.max_vel, config.max_vel);
    acc.right = std::clamp(acc.right, -config.max_vel, config.max_vel);
}

inline void DrawControlUi(ManualControlConfig& config) {
    ImGui::Begin("Control");

    ImGui::Checkbox("Enable manual control", &config.enabled);

    static bool capLInc{ false }, capLDec{ false }, capRInc{ false }, capRDec{ false }, capStop{ false };

    ImGui::BeginDisabled(!config.enabled);

    ImGui::SeparatorText("Parameters");
    double step{ 0.1 };
    ImGui::InputScalar("Max wheel speed (rad/s)", ImGuiDataType_Double, &config.max_vel, &step, nullptr, "%.2f");

    ImGui::SeparatorText("Key Bindings");
    ImGui::TextUnformatted("Left wheel");
    ImGui::SameLine(200);
    ImGui::TextUnformatted("Right wheel");
    ImGui::PushID("row1");
    ImGui::AlignTextToFramePadding();
    ImGui::Text("+");
    ImGui::SameLine();
    KeyBindButton("L+##btn", config.left_inc, capLInc);
    ImGui::SameLine(200);
    ImGui::Text("+");
    ImGui::SameLine();
    KeyBindButton("R+##btn", config.right_inc, capRInc);
    ImGui::PopID();

    ImGui::PushID("row2");
    ImGui::AlignTextToFramePadding();
    ImGui::Text("-"); ImGui::SameLine();
    KeyBindButton("L-##btn", config.left_dec,  capLDec);
    ImGui::SameLine(200);
    ImGui::Text("-"); ImGui::SameLine();
    KeyBindButton("R-##btn", config.right_dec, capRDec);
    ImGui::PopID();

    ImGui::EndDisabled();

    ImGui::End();
}
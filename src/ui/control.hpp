#pragma once

#include <iostream>
#include "imgui.h"

struct ManualControlConfig {
    bool enabled = false;

    ImGuiKey left_inc = ImGuiKey_W;
    ImGuiKey left_dec = ImGuiKey_S;
    ImGuiKey right_inc = ImGuiKey_I;
    ImGuiKey right_dec = ImGuiKey_K;
    ImGuiKey stop_all = ImGuiKey_Space;
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

inline void DrawControlUi(ManualControlConfig& config) {
    ImGui::Begin("Control");

    ImGui::Checkbox("Enable manual control", &config.enabled);

    static bool capLInc{ false }, capLDec{ false }, capRInc{ false }, capRDec{ false }, capStop{ false };

    ImGui::BeginDisabled(!config.enabled);

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
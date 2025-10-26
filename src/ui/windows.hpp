#pragma once

#include "imgui.h"
#include "nfd.h"
#include "core/mouse.hpp"
#include "maze_io.hpp"
#include <fstream>

static std::string mazePath = "";

inline void DrawMouseWindow(Mouse& mouse) {
    ImGui::Begin("Mouse");

    ImGui::Text("Pose x=%.3f, y=%.3f, heading=%.2f", mouse.get_pos_x(), mouse.get_pos_y(), mouse.get_heading());

    ImGui::SeparatorText("Parameters");
    double step{ 0.01 }; // centimeter
    ImGui::InputScalar("Width (m)", ImGuiDataType_Double, &mouse.width, &step, nullptr, "%.3f");
    ImGui::InputScalar("Length (m)", ImGuiDataType_Double, &mouse.length, &step, nullptr, "%.3f");
    ImGui::InputScalar("Wheel Radius (m)", ImGuiDataType_Double, &mouse.wheel_radius, &step, nullptr, "%.3f");

    ImGui::End();
}

inline void DrawWorldWindow(World& world) {
    ImGui::Begin("World");

    ImGui::Text("Maze Path: %s", mazePath.c_str());

    ImGui::End();
}

inline void DrawMenuBar(World& world) {
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Open Maze File...")) {
                nfdchar_t* outPath = nullptr;
                const char* filters = "num";
                const char* defaultDir = nullptr;
                nfdresult_t res = NFD_OpenDialog(filters, defaultDir, &outPath);
                if(res == NFD_OKAY) {                    
                    std::ifstream file(outPath);
                    if (!file.is_open()) {
                        std::cerr << "Failed to open maze file: " << outPath << "\n";
                    } else {
                        mazePath = outPath;
                        Maze maze = read_maze(file);
                        world.maze = maze;
                    }
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
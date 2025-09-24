#pragma once

#include "imgui.h"
#include "core/maze.hpp"
#include "core/mouse.hpp"

constexpr float cellWidthM{ 0.18 };
constexpr float mazePaddingPx{ 16 };
constexpr float wallWidthPx{ 2 };

void draw_maze(ImDrawList* dl, const Maze& maze, ImVec2 tl, float sidePx);
void draw_mouse(ImDrawList* dl, const Maze& maze, const Mouse& mouse, ImVec2 tl, float sidePx);
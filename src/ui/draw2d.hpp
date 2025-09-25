#pragma once

#include "imgui.h"
#include "core/world.hpp"

constexpr float mouseWidth{ 20 };
constexpr float mouseLength{ 20 };

constexpr float cellWidthM{ 0.18 };
constexpr float wallWidthM{ 0.012 };
constexpr float mazePaddingPx{ 16 };

void draw_world(ImDrawList* dl, const World& world, ImVec2 tl, float sidePx);
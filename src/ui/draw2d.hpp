#ifndef DRAW2D_H
#define DRAW2D_H

#include "imgui.h"
#include "core/maze.hpp"

void drawMaze(ImDrawList* dl, const Maze& maze, ImVec2 tl, float sidePx);

#endif
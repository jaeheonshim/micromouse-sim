#include "ui/draw2d.hpp"

void drawMaze(ImDrawList* dl, const Maze& maze, ImVec2 tl, float sidePx) {
    ImVec2 sz{sidePx, sidePx};
    dl->AddRectFilled(tl, ImVec2(tl.x + sz.x, tl.y + sz.y), IM_COL32(25, 25, 30, 255), 6.0f);

    float padding{ 8 };
    float wallWidth{ 2 };
    float cellWidth{ (sidePx - padding * 2) / maze.size };

    for(int y{ 0 }; y < maze.size; ++y) {
        for(int x{ 0 }; x < maze.size; ++x) {
            float cellX{ padding + x * cellWidth }, cellY{ padding + y * cellWidth };
            dl->AddRectFilled(ImVec2(tl.x + cellX - wallWidth / 2, tl.y + cellY - wallWidth / 2), ImVec2(tl.x + cellX + wallWidth / 2, tl.y + cellY + wallWidth / 2), IM_COL32(255, 255, 255, 255), 0.0f);
            dl->AddRectFilled(ImVec2(tl.x + cellX + cellWidth - wallWidth / 2, tl.y + cellY - wallWidth / 2), ImVec2(tl.x + cellX + cellWidth + wallWidth / 2, tl.y + cellY + wallWidth / 2), IM_COL32(255, 255, 255, 255), 0.0f);
            dl->AddRectFilled(ImVec2(tl.x + cellX + cellWidth - wallWidth / 2, tl.y + cellY + cellWidth - wallWidth / 2), ImVec2(tl.x + cellX + cellWidth + wallWidth / 2, tl.y + cellY + cellWidth + wallWidth / 2), IM_COL32(255, 255, 255, 255), 0.0f);
            dl->AddRectFilled(ImVec2(tl.x + cellX - wallWidth / 2, tl.y + cellY + cellWidth - wallWidth / 2), ImVec2(tl.x + cellX + wallWidth / 2, tl.y + cellY + cellWidth + wallWidth / 2), IM_COL32(255, 255, 255, 255), 0.0f);
        }
    }
}
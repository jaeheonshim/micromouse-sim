#include "ui/draw2d.hpp"

void draw_maze(ImDrawList* dl, const Maze& maze, ImVec2 tl, float sidePx) {
    ImVec2 sz{sidePx, sidePx};
    dl->AddRectFilled(tl, ImVec2(tl.x + sz.x, tl.y + sz.y), IM_COL32(25, 25, 30, 255), 6.0f);

    float cellWidth{ (sidePx - mazePaddingPx * 2) / maze.size };

    for(int y{ 0 }; y < maze.size; ++y) {
        for(int x{ 0 }; x < maze.size; ++x) {
            float cellX{ mazePaddingPx + x * cellWidth }, cellY{ mazePaddingPx + y * cellWidth };
            dl->AddRectFilled(ImVec2(tl.x + cellX - wallWidthPx / 2, tl.y + cellY - wallWidthPx / 2), ImVec2(tl.x + cellX + wallWidthPx / 2, tl.y + cellY + wallWidthPx / 2), IM_COL32(255, 255, 255, 255), 0.0f);
            dl->AddRectFilled(ImVec2(tl.x + cellX + cellWidth - wallWidthPx / 2, tl.y + cellY - wallWidthPx / 2), ImVec2(tl.x + cellX + cellWidth + wallWidthPx / 2, tl.y + cellY + wallWidthPx / 2), IM_COL32(255, 255, 255, 255), 0.0f);
            dl->AddRectFilled(ImVec2(tl.x + cellX + cellWidth - wallWidthPx / 2, tl.y + cellY + cellWidth - wallWidthPx / 2), ImVec2(tl.x + cellX + cellWidth + wallWidthPx / 2, tl.y + cellY + cellWidth + wallWidthPx / 2), IM_COL32(255, 255, 255, 255), 0.0f);
            dl->AddRectFilled(ImVec2(tl.x + cellX - wallWidthPx / 2, tl.y + cellY + cellWidth - wallWidthPx / 2), ImVec2(tl.x + cellX + wallWidthPx / 2, tl.y + cellY + cellWidth + wallWidthPx / 2), IM_COL32(255, 255, 255, 255), 0.0f);
        }
    }
}

void draw_mouse(ImDrawList* dl, const Mouse& mouse, ImVec2 tl, float sidePx) {
    dl->AddCircleFilled(ImVec2(tl.x + mazePaddingPx + mouse.get_pos_x(), tl.y + sidePx - (mazePaddingPx + mouse.get_pos_y())), 5, IM_COL32(255, 0, 0, 255));
}
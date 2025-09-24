#include "ui/draw2d.hpp"
#include <cmath>

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

void draw_mouse(ImDrawList* dl, const Maze& maze, const Mouse& mouse, ImVec2 tl, float sidePx) {
    float pixels_per_meter{ sidePx / maze.size / cellWidthM };

    float mouse_pixel_width{ static_cast<float>(mouse.width * pixels_per_meter) };
    float mouse_pixel_length{ static_cast<float>(mouse.length * pixels_per_meter) };
    double x_pixel_pos{ tl.x + mazePaddingPx + mouse.get_pos_x() * pixels_per_meter }, y_pixel_pos{ tl.y + sidePx - (mazePaddingPx + mouse.get_pos_y() * pixels_per_meter) };

    float rect_part_length{ mouse_pixel_length - mouse_pixel_width / 2.0f };
    float rect_part_y_offset{ mouse_pixel_width / 4.0f };

    ImVec2 points[5] {
        ImVec2(rect_part_length/2.0f-rect_part_y_offset, -mouse_pixel_width/2.0f), // FL
        ImVec2(rect_part_length/2.0f-rect_part_y_offset, mouse_pixel_width/2.0f), // FR
        ImVec2(-(rect_part_length/2.0f+rect_part_y_offset), -mouse_pixel_width/2.0f), // BL
        ImVec2(-(rect_part_length/2.0f+rect_part_y_offset), +mouse_pixel_width/2.0f), // BR
        ImVec2(rect_part_length / 2.0f - rect_part_y_offset, 0.0f)
    };

    // Apply rotation and transform into absolute coordinates

    float angle = mouse.get_heading();
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    for (int i = 0; i < 5; i++) {
        float x = points[i].x;
        float y = points[i].y;
        points[i].x = x * cosA - y * sinA + x_pixel_pos;
        points[i].y = -(x * sinA + y * cosA) + y_pixel_pos;
    }

    dl->AddQuadFilled(points[0], points[1], points[3], points[2], IM_COL32(0, 0, 255, 255));
    dl->AddCircleFilled(points[4], mouse_pixel_width / 2.0f + 0.5f, IM_COL32(0, 0, 255, 255));
}
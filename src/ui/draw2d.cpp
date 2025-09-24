#include "ui/draw2d.hpp"
#include "core/maze.hpp"
#include <cmath>

void draw_maze(ImDrawList* dl, const Maze& maze, ImVec2 tl, float sidePx) {
    float pixels_per_meter{ sidePx / maze.size / cellWidthM };

    ImVec2 sz{sidePx, sidePx};
    dl->AddRectFilled(tl, ImVec2(tl.x + sz.x, tl.y + sz.y), IM_COL32(25, 25, 30, 255), 6.0f);

    float cellWidth{ (sidePx - mazePaddingPx * 2) / maze.size };
    float wall_width_px{  wallWidthM * pixels_per_meter };

    auto snap = [](ImVec2 p) {
        p.x = floorf(p.x) + 0.5f;
        p.y = floorf(p.y) + 0.5f;
        return p;
    };


    auto draw_point = [&](const ImVec2& vec) {
        dl->AddRectFilled(snap(ImVec2(vec.x - wall_width_px / 2, vec.y - wall_width_px / 2)), snap(ImVec2(vec.x + wall_width_px / 2, vec.y + wall_width_px / 2)), IM_COL32(255, 255, 255, 255), 0.0f);
    };

    auto draw_wall = [&](const ImVec2& a, const ImVec2& b) {
        // just handle both cases

        if(b.y > a.y) {
            dl->AddQuadFilled(
                snap(ImVec2(a.x - wall_width_px / 2, a.y - wall_width_px / 2)),
                snap(ImVec2(a.x + wall_width_px / 2, a.y - wall_width_px / 2)),
                snap(ImVec2(b.x + wall_width_px / 2, b.y + wall_width_px / 2)),
                snap(ImVec2(b.x - wall_width_px / 2, b.y + wall_width_px / 2)),
                IM_COL32(255, 0, 0, 255)
            );
        } else {
            dl->AddQuadFilled(
                snap(ImVec2(a.x - wall_width_px / 2, a.y - wall_width_px / 2)),
                snap(ImVec2(a.x - wall_width_px / 2, a.y + wall_width_px / 2)),
                snap(ImVec2(b.x + wall_width_px / 2, b.y + wall_width_px / 2)),
                snap(ImVec2(b.x + wall_width_px / 2, b.y - wall_width_px / 2)),
                IM_COL32(255, 0, 0, 255)
            );
        }
    };


    for(int y{ 0 }; y < maze.size; ++y) {
        for(int x{ 0 }; x < maze.size; ++x) {
            float cellX{ mazePaddingPx + x * cellWidth }, cellY{ mazePaddingPx + y * cellWidth };
            const uint8_t cell{ maze.at(x, y) };

            ImVec2 wall_tl(tl.x + cellX, tl.y + cellY + cellWidth);
            ImVec2 wall_tr(tl.x + cellX + cellWidth, tl.y + cellY + cellWidth);
            ImVec2 wall_bl(tl.x + cellX, tl.y + cellY);
            ImVec2 wall_br(tl.x + cellX + cellWidth, tl.y + cellY);

            if(cell & N) draw_wall(wall_tl, wall_tr);
            if(cell & E) draw_wall(wall_br, wall_tr);
            if(cell & S) draw_wall(wall_bl, wall_br);
            if(cell & W) draw_wall(wall_bl, wall_tl);

            draw_point(wall_tl);
            draw_point(wall_tr);
            draw_point(wall_bl);
            draw_point(wall_br);
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
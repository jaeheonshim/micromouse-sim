#include "ui/draw2d.hpp"
#include "core/maze.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

ImVec2 snap(ImVec2 p) {
    p.x = floorf(p.x) + 0.5f;
    p.y = floorf(p.y) + 0.5f;
    return p;
};

void draw_wall(ImDrawList* dl, float wall_width_px, const ImVec2& a, const ImVec2& b) {
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

bool handle_maze_clicks(ImDrawList* dl, const Maze& maze, ImVec2 tl, float side_px) {
    const float pixels_per_meter = side_px / maze.size / cellWidthM;

    const float cell_px = (side_px - mazePaddingPx * 2.0f) / maze.size;
    const float wall_width_px = wallWidthM * pixels_per_meter;

    ImVec2 mouse = ImGui::GetIO().MousePos;
    ImVec2 local(mouse.x - tl.x, mouse.y - tl.y);

    const float hit = cell_px / 2.0f;

    const float gx{ local.x - mazePaddingPx };
    const float gy{ local.y - mazePaddingPx };

    if (gx < 0 || gy < 0 || gx > maze.size * cell_px || gy > maze.size * cell_px) return false; // outside of maze

    // horizontal walls
    const int horiz_r{ (int)std::clamp(std::round(gy / cell_px), 0.0f, (float)maze.size) };
    const int horiz_c{ (int)std::clamp(std::floor(gx / cell_px), 0.0f, (float)maze.size) };

    // vertical
    const int vert_r{ (int)std::clamp(std::floor(gy / cell_px), 0.0f, (float)maze.size) };
    const int vert_c{ (int)std::clamp(std::round(gx / cell_px), 0.0f, (float)maze.size) };

    const bool downL = ImGui::IsMouseDown(ImGuiMouseButton_Left);

    float horiz_d{ std::abs(gx - (horiz_c + 0.5f) * cell_px) };
    float vert_d{ std::abs(gy - (vert_r + 0.5f) * cell_px) };

    ImVec2 horiz_wall[2] = {
        ImVec2(tl.x + mazePaddingPx + horiz_c * cell_px, tl.y + mazePaddingPx + horiz_r * cell_px),
        ImVec2(tl.x + mazePaddingPx + horiz_c * cell_px + cell_px, tl.y + mazePaddingPx + horiz_r * cell_px),
    };

    ImVec2 vert_wall[2] = {
        ImVec2(tl.x + mazePaddingPx + vert_c * cell_px, tl.y + mazePaddingPx + vert_r * cell_px),
        ImVec2(tl.x + mazePaddingPx + vert_c * cell_px, tl.y + mazePaddingPx + vert_r * cell_px + cell_px),
    };

    float thresh{ cell_px * 0.25f };

    if(horiz_d < thresh || vert_d < thresh) {
        if(horiz_d < vert_d) {
            draw_wall(dl, wall_width_px, horiz_wall[0], horiz_wall[1]);
        } else {
            draw_wall(dl, wall_width_px, vert_wall[0], vert_wall[1]);
        }
    }
}

void draw_maze(ImDrawList* dl, const Maze& maze, ImVec2 tl, float sidePx) {
    float pixels_per_meter{ sidePx / maze.size / cellWidthM };

    handle_maze_clicks(dl, maze, tl, sidePx);

    float cellWidth{ (sidePx - mazePaddingPx * 2) / maze.size };
    float wall_width_px{ wallWidthM * pixels_per_meter };

    auto draw_point = [&](const ImVec2& vec) {
        dl->AddRectFilled(snap(ImVec2(vec.x - wall_width_px / 2, vec.y - wall_width_px / 2)), snap(ImVec2(vec.x + wall_width_px / 2, vec.y + wall_width_px / 2)), IM_COL32(255, 255, 255, 255), 0.0f);
    };

    for(int y{ 0 }; y < maze.size; ++y) {
        for(int x{ 0 }; x < maze.size; ++x) {
            float cellX{ mazePaddingPx + x * cellWidth }, cellY{ mazePaddingPx + y * cellWidth };
            const uint8_t cell{ maze.at(x, y) };

            ImVec2 wall_tl(tl.x + cellX, tl.y + cellY + cellWidth);
            ImVec2 wall_tr(tl.x + cellX + cellWidth, tl.y + cellY + cellWidth);
            ImVec2 wall_bl(tl.x + cellX, tl.y + cellY);
            ImVec2 wall_br(tl.x + cellX + cellWidth, tl.y + cellY);

            if(cell & N) draw_wall(dl, wall_width_px, wall_tl, wall_tr);
            if(cell & E) draw_wall(dl, wall_width_px, wall_br, wall_tr);
            if(cell & S) draw_wall(dl, wall_width_px, wall_bl, wall_br);
            if(cell & W) draw_wall(dl, wall_width_px, wall_bl, wall_tl);

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

void draw_world(ImDrawList* dl, const World& world, ImVec2 tl, float sidePx) {
    draw_maze(dl, world.maze, tl, sidePx);
    draw_mouse(dl, world.maze, world.mouse, tl, sidePx);
}
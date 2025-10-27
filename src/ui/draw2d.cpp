#include "ui/draw2d.hpp"
#include "core/maze.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

static float pixels_per_meter;
static float cell_px;
static float wall_width_px;

ImVec2 snap(ImVec2 p) {
    p.x = floorf(p.x) + 0.5f;
    p.y = floorf(p.y) + 0.5f;
    return p;
};

void set_wall(Maze& maze, int wall_r, int wall_c) {
    if(wall_r == maze.size) {
        
    }
}

void draw_wall(ImDrawList* dl, float wall_width_px, const ImVec2& a, const ImVec2& b, const ImU32 color = IM_COL32(255, 0, 0, 255)) {
    // just handle both cases

    if(b.y > a.y) {
        dl->AddQuadFilled(
            snap(ImVec2(a.x - wall_width_px / 2, a.y - wall_width_px / 2)),
            snap(ImVec2(a.x + wall_width_px / 2, a.y - wall_width_px / 2)),
            snap(ImVec2(b.x + wall_width_px / 2, b.y + wall_width_px / 2)),
            snap(ImVec2(b.x - wall_width_px / 2, b.y + wall_width_px / 2)),
            color
        );
    } else {
        dl->AddQuadFilled(
            snap(ImVec2(a.x - wall_width_px / 2, a.y - wall_width_px / 2)),
            snap(ImVec2(a.x - wall_width_px / 2, a.y + wall_width_px / 2)),
            snap(ImVec2(b.x + wall_width_px / 2, b.y + wall_width_px / 2)),
            snap(ImVec2(b.x + wall_width_px / 2, b.y - wall_width_px / 2)),
            color
        );
    }
};

bool handle_maze_clicks(ImDrawList* dl, const Maze& maze, ImVec2 tl, float side_px) {
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
            draw_wall(dl, wall_width_px, horiz_wall[0], horiz_wall[1], IM_COL32(255, 0, 0, 100));
        } else {
            draw_wall(dl, wall_width_px, vert_wall[0], vert_wall[1], IM_COL32(255, 0, 0, 100));
        }
    }
}

void draw_maze(ImDrawList *dl, const Maze &maze, const Mouse &mouse, ImVec2 tl, float side_px) {
    pixels_per_meter = side_px / maze.size / cellWidthM;
    cell_px = (side_px - mazePaddingPx * 2.0f) / maze.size;
    wall_width_px = wallWidthM * pixels_per_meter;

    handle_maze_clicks(dl, maze, tl, side_px);

    auto draw_point = [&](const ImVec2& vec) {
        dl->AddRectFilled(snap(ImVec2(vec.x - wall_width_px / 2, vec.y - wall_width_px / 2)), snap(ImVec2(vec.x + wall_width_px / 2, vec.y + wall_width_px / 2)), IM_COL32(255, 255, 255, 255), 0.0f);
    };
    auto world_to_screen = [&](float wx, float wy)
    {
        float x_px = tl.x + mazePaddingPx + wx * pixels_per_meter;
        float y_px = tl.y + mazePaddingPx + (maze.size * cellWidthM - wy) * pixels_per_meter;
        return ImVec2(x_px, y_px);
    };

    for(int y{ 0 }; y < maze.size; ++y) {
        for(int x{ 0 }; x < maze.size; ++x) {
            float cellX{ mazePaddingPx + x * cell_px }, cellY{ mazePaddingPx + y * cell_px };
            const uint8_t cell{ maze.at(x, y) };

            ImVec2 wall_tl(tl.x + cellX, tl.y + cellY + cell_px);
            ImVec2 wall_tr(tl.x + cellX + cell_px, tl.y + cellY + cell_px);
            ImVec2 wall_bl(tl.x + cellX, tl.y + cellY);
            ImVec2 wall_br(tl.x + cellX + cell_px, tl.y + cellY);

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
    if (mouse.showRaycast) {
        ImVec2 ray_start = world_to_screen(mouse.get_pos_x(), mouse.get_pos_y());
        ImVec2 ray_end = world_to_screen(mouse.sensorReadings[0].x, mouse.sensorReadings[0].y);

        dl->AddLine(ray_start, ray_end, IM_COL32(255, 0, 0, 255), 2.0f);
        dl->AddCircleFilled(ray_end, 3.0f, IM_COL32(255, 100, 100, 255));
    }
   
}

void draw_mouse(ImDrawList* dl, const Maze& maze, const Mouse& mouse, ImVec2 tl, float sidePx) {
    pixels_per_meter = sidePx / maze.size / cellWidthM;

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
    draw_maze(dl, world.maze, world.mouse, tl, sidePx);
    draw_mouse(dl, world.maze, world.mouse, tl, sidePx);
}
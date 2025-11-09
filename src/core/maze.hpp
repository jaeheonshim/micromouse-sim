#pragma once

#include <cstdint>
#include <vector>
#include "types.hpp"
#include <cmath>
#include <iostream>

enum : uint8_t {
    N = 1<<0,
    E = 1<<1,
    S = 1<<2,
    W = 1<<3
};

struct Maze {
    int size{};
    std::vector<uint8_t> cells;

    Maze(int size = 16, bool blank = false) : size{ size }, cells{ std::vector<uint8_t>(size * size) } {
        if(!blank) {
            for(int i{ 0 }; i < size; ++i) {
                at(0, i) |= W;
                at(size-1, i) |= E;
                at(i, 0) |= S;
                at(i, size-1) |= N;
            }
            at(1,9) |= N;
        }
    }
    bool has_wall(int cx, int cy, uint8_t side) const {
        if (cx < 0 || cx >= size || cy < 0 || cy >= size) return true;
        return (at(cx, cy) & side) != 0;
    }

    uint8_t& at(int x, int y){
        return cells[y * size + x];
    }

    const uint8_t& at(int x,int y) const {
        return cells[y * size + x];
    }

    Pose raycast(Pose origin) {
        constexpr double cell_width = 0.18;
        constexpr double EPS = 1e-9;
        double x = origin.x, y = origin.y;
        double dx = std::cos(origin.th), dy = std::sin(origin.th);
        
        int cx = static_cast<int>(std::floor(x / cell_width));
        int cy = static_cast<int>(std::floor(y / cell_width));

        int stepX = (dx > 0) - (dx < 0); // 1 or -1
        int stepY = (dy > 0) - (dy < 0);
        const double INF = std::numeric_limits<double>::infinity();
        double nextGridX = (stepX > 0) ? ( (cx + 1) * cell_width ) : ( cx * cell_width );
        double nextGridY = (stepY > 0) ? ( (cy + 1) * cell_width ) : ( cy * cell_width );

        double tMaxX  = (std::abs(dx) > EPS) ? ( (nextGridX - x) / dx ) : INF; // the total distance along the ray to hit the next vertical line
        double tMaxY  = (std::abs(dy) > EPS) ? ( (nextGridY - y) / dy ) : INF; // the total distance along the ray to hit the next horizontal line

        double tDeltaX = (std::abs(dx) > EPS) ? ( cell_width / std::abs(dx) ) : INF; // the distance you move along the ray to travel delta_x = cell_width
        double tDeltaY = (std::abs(dy) > EPS) ? ( cell_width / std::abs(dy) ) : INF; // the distance you move along the ray to travel delta_y = cell_width

        int maxIters = size * size * 4;

        for (int i = 0; i < maxIters; ++i) {
            // if the next vertical line comes up sooner than the next horizontal line
            if (tMaxX < tMaxY) {
                uint8_t side = (stepX > 0) ? E : W;
                if (has_wall(cx, cy, side)) {
                    std::cout << cx << ' ' << cy << '\n';
                    double t = tMaxX;
                    return { x + dx * t, y + dy * t, 0.0 };
                }
                cx += stepX;
                tMaxX += tDeltaX; // move the distance along ray to next vertical line by tDeltaX
            } else {
                uint8_t side = (stepY > 0) ? N : S;
                if (has_wall(cx, cy, side)) {
                    double t = tMaxY;
                    return { x + dx * t, y + dy * t, 0.0 };
                }
                cy += stepY;
                tMaxY += tDeltaY;
            }
        }
        return {origin.x, origin.y, 0.0};
    }
};
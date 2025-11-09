#pragma once

#include <cstdint>
#include <vector>
#include <string>

struct PathDrawCommand {
    int dx;
    int dy;
    uint8_t edge_end;
};

struct PathDrawPoint {
    int x;
    int y;
    uint8_t edge;
};
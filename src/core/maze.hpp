#pragma once

#include <cstdint>
#include <vector>

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
        }
    }

    uint8_t& at(int x, int y){
        return cells[y * size + x];
    }

    const uint8_t& at(int x,int y) const {
        return cells[y * size + x];
    }
};
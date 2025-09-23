#ifndef MAZE_H
#define MAZE_H

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

    Maze(int size = 16) : size{ size }, cells{ std::vector<uint8_t>(size * size) } {

    }

    uint8_t& at(int x,int y){
        return cells[y * size + x];
    }

    const uint8_t& at(int x,int y) const {
        return cells[y * size + x];
    }
};

#endif
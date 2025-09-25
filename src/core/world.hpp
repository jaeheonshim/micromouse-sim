#pragma once

#include "core/maze.hpp"
#include "core/mouse.hpp"

struct World {
    Maze maze;
    Mouse mouse;

    void step(double dt);
};
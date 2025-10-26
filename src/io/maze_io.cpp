#include "maze_io.hpp"

Maze read_maze(std::istream& is) {
    Maze maze(16, true);

    for(int i = 0; i < 256; ++i) {
        int x, y, n, e, s, w;
        is >> x >> y >> n >> e >> s >> w;
        uint8_t walls = 0;
        if(n) walls |= N;
        if(s) walls |= S;
        if(e) walls |= E;
        if(w) walls |= W;
        maze.at(x, y) = walls;
    }

    return maze;
}
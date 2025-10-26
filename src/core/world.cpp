#include "core/world.hpp"

void World::step(double dt) {
    this->mouse.step(dt);
    for (int i = 0; i < this->mouse.sensors.size(); i++){
        this->maze.raycast(this->mouse.sensors.at(i) + this->mouse.pose);
    }
}
#include "core/world.hpp"
#include "types.hpp"
void World::step(double dt) {
    this->mouse.step(dt);
    this->mouse.sensorReadings.clear();
    for (int i = 0; i < this->mouse.sensors.size(); i++)
    {
        Pose x = this->maze.raycast(this->mouse.sensors.at(i) + this->mouse.pose);
        this->mouse.sensorReadings.push_back(x);
    }
}
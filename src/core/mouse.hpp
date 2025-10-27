#pragma once

#include <vector>
#include "core/types.hpp"
#include <cmath>

// Represents the physical state of a micromouse within the world
struct Mouse {
  double width{ 0.08 };
  double length{ 0.1 };
  double wheel_radius{ 0.02 };

  bool showRaycast; 

  Pose pose{ 0.09, 0.09, M_PI / 2 };

  std::vector<Pose> sensors{{0, 0, 0}};
  std::vector<Pose> sensorReadings{};

  // velocities of the wheels
  double w_l{ 0 };
  double w_r{ 0 };

  void step(double dt);

  double get_pos_x() const {
    return pose.x;
  }

  double get_pos_y() const {
    return pose.y;
  }

  double get_heading() const {
    return pose.th;
  }

  void set_wheels_vel(double w_l, double w_r) {
    this->w_l = w_l;
    this->w_r = w_r;
  }
};
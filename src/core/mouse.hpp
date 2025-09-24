#pragma once

#include <vector>
#include "core/types.hpp"

// Represents the physical state of a micromouse within the world
class Mouse {
  Pose pose{};

  double width{ 0.05 };
  double wheel_radius{ 0.02 };

  // velocities of the wheels
  double w_l{ 0 };
  double w_r{ 0 };

  
public:
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
#pragma once

#include <vector>
#include "core/types.hpp"

// Represents the physical state of a micromouse within the world
class Mouse {
  Pose pose;

  double width;
  double wheel_radius;

  // velocities of the wheels
  double w_l;
  double w_r;

  void step(double dt);

public:
  double get_pos_x() const {
    return pose.x;
  }

  double get_pos_y() const {
    return pose.y;
  }
};
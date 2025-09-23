#ifndef MOUSE_H
#define MOUSE_H

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
};

#endif
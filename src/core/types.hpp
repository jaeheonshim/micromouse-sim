#pragma once

struct Pose {
  double x{0}, y{0}, th{0.0};
};
inline Pose operator+(const Pose p1, const Pose p2) {
  return {p1.x + p2.x, p1.y + p2.y};
}

struct Twist {
  double vx{0.0}, vy{0.0}, wz{0.0};
};
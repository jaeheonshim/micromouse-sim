#ifndef TYPES_H
#define TYPES_H

struct Pose {
  double x{0}, y{0}, th{0.0};
};

struct Twist {
  double vx{0.0}, vy{0.0}, wz{0.0};
};

#endif
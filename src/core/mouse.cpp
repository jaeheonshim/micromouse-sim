#include <iostream>
#include "core/mouse.hpp"
#include <cmath>

Twist solve_twist_from_wheels(double w_l, double w_r, double b, double r) {
    Twist q;
    q.vx = 0.5 * r * (w_r + w_l);
    q.vy = 0.0;
    q.wz = (r / b) * (w_r - w_l);
    return q;
}

void Mouse::step(double dt) {
    Twist q = solve_twist_from_wheels(this->w_l, this->w_r, this->width, this->wheel_radius);

    this->pose.x += q.vx * cos(this->pose.th) * dt;
    this->pose.y += q.vx * sin(this->pose.th) * dt;
    this->pose.th += q.wz * dt;
}
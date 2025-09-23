#include "core/mouse.hpp"

Twist solve_twist_from_wheels(double w_l, double w_r, double b) {
    return Twist{};
}

void Mouse::step(double dt) {
    Twist q = solve_twist_from_wheels(this->w_l, this->w_r, this->width);
}
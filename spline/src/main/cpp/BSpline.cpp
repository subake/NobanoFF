#include "BSpline.h"
#include <iostream>

BSpline::BSpline() : Curve() {}

BSpline::~BSpline() {}

void BSpline::_on_way_point_added() {
    if (_way_points.size() < 4) {
        return;
    }

    int new_control_point_index = static_cast<int>(_way_points.size()) - 1;

    int pt = new_control_point_index - 3;

    for (int i = 0; i <= _steps; i++) {
        double u = (double)i / (double)_steps;
        add_node(interpolate_6_lambda(u, _way_points[pt], _way_points[pt + 1], _way_points[pt + 2], _way_points[pt + 3], 1));
    }
}

Vector BSpline::interpolate_3(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3) {
    Vector point;
    point =  u*u*u*((-1) * P0 + 3 * P1 - 3 * P2 + 1 * P3) / 6;
    point +=   u*u*(   3 * P0 - 6 * P1 + 3 * P2 + 0 * P3) / 6;
    point +=     u*((-3) * P0 + 0 * P1 + 3 * P2 + 0 * P3) / 6;
    point +=       (   1 * P0 + 4 * P1 + 1 * P2 + 0 * P3) / 6;

    return point;
}

Vector BSpline::interpolate_4(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3, const Vector& P4) {
    Vector point;
    point =  u*u*u*u*(   1 * P0 -  4 * P1 +  6 * P2 - 4 * P3 + 1 * P4) / 24;
    point +=   u*u*u*((-4) * P0 + 12 * P1 - 12 * P2 + 4 * P3 + 0 * P4) / 24;
    point +=     u*u*(   6 * P0 -  6 * P1 -  6 * P2 + 6 * P3 + 0 * P4) / 24;
    point +=       u*((-4) * P0 - 12 * P1 + 12 * P2 + 4 * P3 + 0 * P4) / 24;
    point +=         (   1 * P0 + 11 * P1 + 11 * P2 + 1 * P3 + 0 * P4) / 24;

    return point;
}

Vector BSpline::interpolate_4_lambda(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3, double lambda) {
    Vector point;
    point =  P0 * (4 - lambda - 3 * lambda * u) * (1 - u) * (1 - u) * (1 - u) / 24;
    point += P1 * (16 + 2 * lambda - 12 * (2 + lambda) * u*u + 12 * (1 + lambda) * u*u*u - 3 * lambda * u*u*u*u) / 24;
    point += P2 * (4 - lambda + 12 * u + 6 * (2 + lambda) * u*u - 12 * u*u*u - 3 * lambda * u*u*u*u) / 24;
    point += P3 * (4 * (1 - lambda) + 3 * lambda * u) * u*u*u / 24;

    return point;
}

Vector BSpline::interpolate_5_lambda(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3, double lambda) {
    Vector point;
    point =  P0 * (5 - lambda - 4 * lambda * u) * (1 - u) * (1 - u) * (1 - u) * (1 - u) / 40;
    point += P1 * (30 + 2 * lambda - 20 * (3 + lambda) * u*u + 40 * (1 + lambda) * u*u*u - 5 * (1 + 7 * lambda) * u*u*u*u + 12 * lambda * u*u*u*u*u) / 40;
    point += P2 * (5 - lambda + 20 * u + 10 * (3 + lambda) * u*u - 20 * (1 + lambda) * u*u*u - 5 * (1 - 5 * lambda) * u*u*u*u - 12 * lambda * u*u*u*u*u) / 40;
    point += P3 * (5 * (1 - lambda) + 4 * lambda * u) * u*u*u*u / 40;

    return point;
}

Vector BSpline::interpolate_6_lambda(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3, double lambda) {
    Vector point;
    point =  P0 * (6 - lambda - 5 * lambda * u) * (1 - u) * (1 - u) * (1 - u) * (1 - u) * (1 - u) / 60;
    point += P1 * (48 + 2 * lambda - 30 * (4 + lambda) * u*u + 40 * (3 + 2 * lambda) * u*u*u - 30 * (2 + 3 * lambda) * u*u*u*u + 6 * (3 + 7 * lambda) * u*u*u*u*u - 5 * lambda * u*u*u*u*u*u) / 60;
    point += P2 * (6 - lambda + 30 * u + 15 * (4 + lambda) * u*u - 20 * (3 + 2 * lambda) * u*u*u + 15 * (2 + 3 * lambda) * u*u*u*u - 6 * (3 + 2 * lambda) * u*u*u*u*u - 5 * lambda * u*u*u*u*u*u) / 60;
    point += P3 * (6 * (1 - lambda) + 5 * lambda * u) * u*u*u*u*u / 60;

    return point;
}


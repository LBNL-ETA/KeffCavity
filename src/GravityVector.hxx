#pragma once

namespace FenestrationCommon {
//! Storage for unity vector that represents gravity direction
    struct GravityVector {
        GravityVector() : x{0}, y{-1}, z{0} {}

        GravityVector(const double x, const double y, const double z) : x(x), y(y), z(z) {}

        double x;
        double y;
        double z;
    };
}
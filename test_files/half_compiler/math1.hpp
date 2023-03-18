#pragma once

#include "primitives.hpp"

#define PI 3.14

namespace math1 {

  namespace types {
    template <typename Ty>
    struct Point2D { Ty x, y; };

    template <typename Ty>
    struct Point3D { f64 x, y, z; };

    using Point2Di32 = Point2D<i32>;
  }

  namespace types2 = types;

  template <typename Ty>
  Ty add(Ty const &a, Ty const &b) {
    return a + b;
  }

} // namespace math

#pragma once

#include <concepts>
#include <functional>

typedef float real32;
typedef double real64;

namespace math2 {

  #define PI 3.1415

  template<typename T>
  concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
  };

  template <Hashable Ty>
  Ty add_hashable(Ty const &a, Ty const &b) {
    return a + b;
  }

}

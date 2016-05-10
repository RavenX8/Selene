#pragma once
#include <tuple>

/*
 * Implements various type trait objects
 */

namespace sel {
namespace detail {

template <typename T>
struct _arity {
# if _MSC_VER <= 1800
  _arity() {
    value = 1;
  }
  static int value;
#else
  static constexpr int value = 1;
#endif
};

template <typename... Vs>
struct _arity<std::tuple<Vs...>> {
# if _MSC_VER <= 1800
  _arity() {
    value = sizeof...(Vs);
  }
  static int value;
#else
  static constexpr int value = sizeof...(Vs);
#endif
};

template <>
struct _arity<void> {
# if _MSC_VER <= 1800
  _arity<void>(){
    value = 0;
  }
  static int value;
#else
  static constexpr int value = 0;
#endif
};

template <std::size_t... Is>
struct _indices {};

template <std::size_t N, std::size_t... Is>
struct _indices_builder : _indices_builder<N-1, N-1, Is...> {};

template <std::size_t... Is>
struct _indices_builder<0, Is...> {
    using type = _indices<Is...>;
};

template <typename T> struct _id {};
}
}

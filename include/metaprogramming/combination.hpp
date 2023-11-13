#pragma once

template <int N, int K>
struct Combination {
  static constexpr int value =
      Combination<N - 1, K - 1>::value + Combination<N - 1, K>::value;
};

template <int N>
struct Combination<N, 0> {
  static constexpr int value = 1;
};

template <int N>
struct Combination<N, N> {
  static constexpr int value = 1;
};

template <>
struct Combination<0, 0> {
  static constexpr int value = 1;
};

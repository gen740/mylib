#pragma once
#include <type_traits>

template <int NMax, int MMax, int N = 0, int M = 0>
struct ForMN {
  template <template <int I, int J> class T, typename... Args>
  __attribute__((always_inline)) static void EvalAll(Args &&...args) {
    T<N, M>::eval(std::forward<Args>(args)...);
    if constexpr (N == NMax - 1 && M == MMax - 1) {
      return;
    } else if constexpr (M == MMax - 1) {
      ForMN<NMax, MMax, N + 1, N>::template EvalAll<T>(
          std::forward<Args>(args)...);
    } else {
      ForMN<NMax, MMax, N, M + 1>::template EvalAll<T>(
          std::forward<Args>(args)...);
    }
  }

  template <template <int I, int J> class T, typename... Args>
  __attribute__((always_inline)) static void EvalHalf(Args &&...args) {
    T<N, M>::eval(std::forward<Args>(args)...);
    if constexpr (N == NMax - 1 && M == MMax - 1) {
      return;
    } else if constexpr (M == N) {
      ForMN<NMax, MMax, N + 1, 0>::template EvalHalf<T>(
          std::forward<Args>(args)...);
    } else {
      ForMN<NMax, MMax, N, M + 1>::template EvalHalf<T>(
          std::forward<Args>(args)...);
    }
  }
};

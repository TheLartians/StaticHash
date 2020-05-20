#pragma once

#include <array>
#include <climits>

namespace static_hash {

 template <class T, bool ALLOW_PADDING = false, class C, size_t N> constexpr auto convertBinaryArrayType(std::array<C, N> input) {
    static_assert(std::is_unsigned<T>::value);
    static_assert(std::is_unsigned<C>::value);
    if constexpr (sizeof(T) > sizeof(C)) {
      constexpr auto ratio = sizeof(T)/sizeof(C);
      constexpr auto padding = input.size() % ratio == 0 ? 0 : 1;
      static_assert(ALLOW_PADDING || (padding == 0));
      std::array<T, input.size() / ratio + padding> arr{};
      for (size_t i=0; i<arr.size(); ++i) {
        arr[i] = 0;
        for (size_t j=0; j<ratio; ++j) {
          auto idx = i*ratio+j;
          auto offset = (ratio - j - 1) * sizeof(C) * CHAR_BIT;
          auto v = idx < input.size() ? input[idx] : T(0);
          arr[i] |= v << offset;
        }
      }
      return arr;
    } else {
      constexpr auto ratio = sizeof(C)/sizeof(T);
      std::array<T, input.size() * ratio> arr{};
      for (size_t i=0; i<arr.size(); ++i) {
        auto j = i % ratio;
        arr[i] = C(input[i/ratio]) >> ((ratio - j - 1)  * sizeof(T) * CHAR_BIT);
      }
      return arr;
    }
  }

}
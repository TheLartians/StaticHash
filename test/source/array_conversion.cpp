#include <doctest/doctest.h>
#include <static_hash/util/array_conversion.h>

template <class T1, class T2> auto checkArrayConversion() {
  constexpr auto a = std::array<T1, 8>(
      {{0, 1, 2, 3, std::numeric_limits<T1>::max() / 2, std::numeric_limits<T1>::max() - 1,
        std::numeric_limits<T1>::max() - 2, std::numeric_limits<T1>::max() - 3}});
  constexpr auto converted = static_hash::convertBinaryArrayType<T2>(a);
  constexpr auto back = static_hash::convertBinaryArrayType<T1>(converted);
  CHECK(a == back);
}

TEST_CASE("SHA256") {
  checkArrayConversion<unsigned char, unsigned int>();
  checkArrayConversion<unsigned int, unsigned char>();
  checkArrayConversion<unsigned char, unsigned long long>();
  checkArrayConversion<unsigned long long, unsigned char>();
}

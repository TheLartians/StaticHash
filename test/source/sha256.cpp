#include <doctest/doctest.h>
#include <static_hash/sha256.h>

#include <string>
#include <vector>

inline std::string string_to_hex(std::string_view input) {
  static const char hex_digits[] = "0123456789abcdef";

  std::string output;
  output.reserve(input.length() * 2);
  for (unsigned char c : input) {
    output.push_back(hex_digits[c >> 4]);
    output.push_back(hex_digits[c & 15]);
  }
  return output;
}

inline std::string hash_to_string(const static_hash::SHA256 &hash) {
  auto le = hash.asLittleEndian();
  return string_to_hex(std::string_view(reinterpret_cast<const char *>(le.data()),
                                        le.size() * sizeof(decltype(le)::value_type)));
}

template <size_t N> inline constexpr auto toArray(std::string_view input) {
  std::array<unsigned char, N> inputVector;
  for (size_t i = 0; i < N; ++i) inputVector[i] = input[i];
  return input;
}

TEST_CASE("SHA256 consistency") {
  using namespace static_hash;
  constexpr std::string_view input = "Hello, World!";
  std::string reference = "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f";

  SUBCASE("from string_view") {
    constexpr auto hash = static_hash::sha256(std::string_view(input));
    CHECK(hash_to_string(hash) == reference);
  }

  SUBCASE("from string") {
    auto hash = static_hash::sha256(std::string(input));
    CHECK(hash_to_string(hash) == reference);
  }

  SUBCASE("from vector") {
    std::vector<unsigned char> inputVector(input.begin(), input.end());
    auto hash = static_hash::sha256(inputVector);
    CHECK(hash_to_string(hash) == reference);
  }

  SUBCASE("from array") {
    auto inputArray = toArray<input.size()>("Hello, World!");
    auto hash = static_hash::sha256(inputArray);
    CHECK(hash_to_string(hash) == reference);
  }
}

TEST_CASE("SHA256 constexpr construction and comparison") {
  constexpr auto hash1 = static_hash::sha256(std::array<char, 5>{1, 2, 3, 4, 5});
  constexpr auto hash2 = static_hash::sha256(std::array<char, 5>{1, 3, 2, 4, 5});
  static_assert(hash1 == hash1);
  static_assert(hash2 == hash2);
  static_assert(hash1 != hash2);
}

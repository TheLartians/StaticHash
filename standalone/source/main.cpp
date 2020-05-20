#include <static_hash.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, static_hash::LanguageCode> languages{
    {"en", static_hash::LanguageCode::EN},
    {"de", static_hash::LanguageCode::DE},
    {"es", static_hash::LanguageCode::ES},
    {"fr", static_hash::LanguageCode::FR},
};

int main(int argc, char** argv) {
  cxxopts::Options options(argv[0], "A program to welcome the world!");

  std::string language;
  std::string name;

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("n,name", "Name to greet", cxxopts::value(name)->default_value("World"))
    ("l,lang", "Language code to use", cxxopts::value(language)->default_value("en"))
  ;
  // clang-format on

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  auto langIt = languages.find(language);
  if (langIt == languages.end()) {
    std::cout << "unknown language code: " << language << std::endl;
    return 1;
  }

  static_hash::StaticHash static_hash(name);
  std::cout << static_hash.greet(langIt->second) << std::endl;

  return 0;
}

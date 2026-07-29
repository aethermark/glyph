#include <iostream>
#include <parser/binary_reader.hpp>
#include <printer/formatter.hpp>
#include <vector>

#include "printer/style.hpp"

auto main() -> int {
  glyph::BinaryReader reader;

  if (!reader.LoadData("tests/assets/JetBrainsMono-Regular.ttf")) {
    std::cerr << "Failed to load font.\n";
    return 1;
  }

  const auto directory = reader.ReadTableDirectory();

  std::vector<glyph::TableRecord> tables;
  tables.reserve(directory.num_tables);

  for (uint16_t i = 0; i < directory.num_tables; ++i) {
    tables.push_back(reader.ReadTableRecord());
  }

  printer::Formatter formatter;

  formatter.ThemeDark();
  formatter.Print(std::cout, directory);

  std::cout << '\n';
  formatter.ThemeGruvbox();
  formatter.Print(std::cout, tables);

  return 0;
}

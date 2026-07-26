#include <iostream>
#include <printer/printer.hpp>
#include <parser/binary_reader.hpp>
#include <vector>

using printer::operator<<;

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

  std::cout << printer::pretty << printer::header_bold << printer::header(printer::Color::kBlue)
            << printer::major_bold << printer::major(printer::Color::kBlue);

  std::cout << directory;
  std::cout << tables;

  return 0;
}

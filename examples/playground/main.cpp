#include <iostream>
#include <parser/binary_reader.hpp>
#include <printer/formatter.hpp>

#include "parser/ttf_parser.hpp"

auto main() -> int {
  glyph::TtfParser parser;

  auto head = parser.ReadHeadTable();
  auto maxp = parser.ReadMaxpTable();
  auto loca = parser.ReadLocaTable();

  printer::Formatter formatter;
  formatter.ThemeGruvbox();
  formatter.Print(std::cout, head);
  formatter.Print(std::cout, maxp);
  formatter.Print(std::cout, loca);

  return 0;
}

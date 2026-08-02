#include <bitset>
#include <iostream>
#include <parser/binary_reader.hpp>
#include <printer/formatter.hpp>
#include <variant>

#include "parser/constants.hpp"
#include "parser/tables.hpp"
#include "parser/ttf_parser.hpp"

auto main() -> int {
  glyph::TtfParser parser;

  glyph::Glyph glyph = parser.ReadGlyph(parser.LookupGlyph(U'B'));

  printer::Formatter formatter;
  formatter.ThemeGruvbox();

  if (std::holds_alternative<glyph::SimpleGlyph>(glyph.data)) {
    const auto& simple = std::get<glyph::SimpleGlyph>(glyph.data);

    std::cout << "=== Header ===\n";
    formatter.Print(std::cout, simple.header);

    std::cout << "\nEnd points:\n";
    for (std::size_t i = 0; i < simple.end_pts_of_contours.size(); ++i) {
      std::cout << i << ": " << simple.end_pts_of_contours[i] << '\n';
    }

    std::cout << "\nInstructions (" << simple.instructions.size() << " bytes):\n";
    for (uint8_t b : simple.instructions) {
      std::cout << std::hex << std::uppercase << static_cast<int>(b) << ' ';
    }
    std::cout << std::dec << "\n";

    std::cout << "\nFlags:\n";
    for (std::size_t i = 0; i < simple.flags.size(); ++i) {
      std::cout << i << ": " << std::bitset<glyph::kBYTE>(static_cast<int>(simple.flags[i]))
                << std::dec << '\n';
    }

    std::cout << "\nPoints:\n";
    for (std::size_t i = 0; i < simple.x_coordinates.size(); ++i) {
      std::cout << i << ": (" << simple.x_coordinates[i] << ", " << simple.y_coordinates[i]
                << ")\n";
    }
  } else {
    const auto& compound = std::get<glyph::CompoundGlyph>(glyph.data);

    std::cout << "=== Header ===\n";
    formatter.Print(std::cout, compound.header);

    std::cout << "\nComponents:\n";
    for (std::size_t i = 0; i < compound.components.size(); ++i) {
      const auto& c = compound.components[i];

      std::cout << "Component " << i << '\n'
                << "  glyph index : " << c.glyph_index << '\n'
                << "  flags       : 0x" << std::hex << c.flags << std::dec << '\n'
                << "  arg1        : " << c.argument1 << '\n'
                << "  arg2        : " << c.argument2 << '\n'
                << "  scale_x     : " << c.scale_x << '\n'
                << "  scale_y     : " << c.scale_y << '\n'
                << "  scale01     : " << c.scale01 << '\n'
                << "  scale10     : " << c.scale10 << "\n\n";
    }

    std::cout << "Instructions (" << compound.instructions.size() << " bytes):\n";
    for (uint8_t b : compound.instructions) {
      std::cout << std::hex << std::uppercase << static_cast<int>(b) << ' ';
    }
    std::cout << std::dec << '\n';
  }

  return 0;
}

#pragma once

#include <cstdint>

#include "parser/binary_reader.hpp"
#include "parser/font.hpp"
#include "parser/tables.hpp"

namespace glyph {

class TtfParser {
public:
  TtfParser();

  auto ReadHeadTable() -> HeadTable;
  auto ReadMaxpTable() -> MaxpTable;
  auto ReadLocaTable() -> LocaTable;
  auto ReadCmapTable() -> CmapTable;

  [[nodiscard]] auto LookupGlyph(char32_t c) const -> uint16_t;

  auto ReadGlyph(uint16_t glyph_index) -> Glyph;

private:
  auto ReadHeader() -> GlyphHeader;
  auto ReadSimpleGlyph(const GlyphHeader& header) -> SimpleGlyph;
  auto ReadCompoundGlyph(const GlyphHeader& header) -> CompoundGlyph;

  BinaryReader reader_;
  Font font_;
  CmapTable cmap_;
};

}  // namespace glyph

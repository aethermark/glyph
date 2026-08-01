#pragma once

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

private:
  BinaryReader reader_;
  Font font_;
};

}  // namespace glyph

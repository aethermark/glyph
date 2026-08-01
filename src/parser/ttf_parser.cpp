#include "parser/ttf_parser.hpp"

#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <vector>

#include "parser/constants.hpp"

namespace glyph {

TtfParser::TtfParser() {
  reader_ = BinaryReader();
  font_ = reader_.ReadFont("tests/assets/JetBrainsMono-Regular.ttf");
}

auto TtfParser::ReadHeadTable() -> HeadTable {
  const TableRecord* table = font_.GetHead();

  if (table == nullptr) {
    throw std::runtime_error("Font does not contain a 'head' table.");
  }

  reader_.Seek(table->offset);

  HeadTable head{};

  head.major_version = reader_.ReadUInt16();
  head.minor_version = reader_.ReadUInt16();
  head.font_revision = reader_.ReadFixed();
  head.checksum_adjustment = reader_.ReadUInt32();
  head.magic_number = reader_.ReadUInt32();
  head.flags = reader_.ReadUInt16();
  head.units_per_em = reader_.ReadUInt16();

  // TODO(MukulWaval): Read LONGDATETIME values.
  reader_.Skip(kBYTE);  // created
  reader_.Skip(kBYTE);  // modified

  head.x_min = reader_.ReadInt16();
  head.y_min = reader_.ReadInt16();
  head.x_max = reader_.ReadInt16();
  head.y_max = reader_.ReadInt16();

  head.mac_style = reader_.ReadUInt16();
  head.lowest_rec_ppem = reader_.ReadUInt16();
  head.font_direction_hint = reader_.ReadInt16();
  head.index_to_loc_format = reader_.ReadInt16();
  head.glyph_data_format = reader_.ReadInt16();

  return head;
}

auto TtfParser::ReadMaxpTable() -> MaxpTable {
  const TableRecord* table = font_.GetMaxp();

  if (table == nullptr) {
    throw std::runtime_error("Font does not contain a 'maxp' table.");
  }

  reader_.Seek(table->offset);

  MaxpTable maxp{};

  maxp.version = reader_.ReadUInt32();
  maxp.num_glyphs = reader_.ReadUInt16();

  const auto version_1 = 0x00010000;

  if (maxp.version == version_1) {
    maxp.max_points = reader_.ReadUInt16();
    maxp.max_contours = reader_.ReadUInt16();
    maxp.max_composite_points = reader_.ReadUInt16();
    maxp.max_composite_contours = reader_.ReadUInt16();
    maxp.max_zones = reader_.ReadUInt16();
    maxp.max_twilight_points = reader_.ReadUInt16();
    maxp.max_storage = reader_.ReadUInt16();
    maxp.max_function_defs = reader_.ReadUInt16();
    maxp.max_instruction_defs = reader_.ReadUInt16();
    maxp.max_stack_elements = reader_.ReadUInt16();
    maxp.max_size_of_instructions = reader_.ReadUInt16();
    maxp.max_component_elements = reader_.ReadUInt16();
    maxp.max_component_depth = reader_.ReadUInt16();
  }
  return maxp;
}

auto TtfParser::ReadLocaTable() -> LocaTable {
  const TableRecord* table = font_.GetLoca();

  if (table == nullptr) {
    throw std::runtime_error("Font does not contain a 'loca' table.");
  }

  reader_.Seek(table->offset);

  LocaTable loca{};

  auto maxp = ReadMaxpTable();
  auto head = ReadHeadTable();

  loca.offsets.reserve(maxp.num_glyphs + 1);

  if (head.index_to_loc_format == 0) {
    for (uint16_t i = 0; i <= maxp.num_glyphs; ++i) {
      loca.offsets.push_back(static_cast<uint32_t>(reader_.ReadUInt16()) * 2);
    }
  } else {
    for (uint16_t i = 0; i <= maxp.num_glyphs; ++i) {
      loca.offsets.push_back(reader_.ReadUInt32());
    }
  }

  return loca;
}

}  // namespace glyph

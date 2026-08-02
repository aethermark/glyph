#include "parser/ttf_parser.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <variant>
#include <vector>

#include "parser/constants.hpp"
#include "parser/tables.hpp"

namespace glyph {
using std::size_t;

TtfParser::TtfParser() {
  reader_ = BinaryReader();
  font_ = reader_.ReadFont("tests/assets/JetBrainsMono-Regular.ttf");
  cmap_ = ReadCmapTable();
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

  LocaTable loca{};

  auto maxp = ReadMaxpTable();
  auto head = ReadHeadTable();

  reader_.Seek(table->offset);

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

auto TtfParser::ReadCmapTable() -> CmapTable {
  CmapTable cmap{};

  const TableRecord* table = font_.FindTable("cmap");
  reader_.Seek(table->offset);
  const auto table_start = table->offset;

  cmap.version = reader_.ReadUInt16();
  cmap.num_tables = reader_.ReadUInt16();

  cmap.encodings.reserve(cmap.num_tables);

  for (uint16_t i = 0; i < cmap.num_tables; ++i) {
    CmapEncodingRecord record{};

    record.platform_id = reader_.ReadUInt16();
    record.encoding_id = reader_.ReadUInt16();
    record.offset = reader_.ReadUInt32();

    cmap.encodings.push_back(record);
  }

  for (const auto& encoding : cmap.encodings) {
    // Windows Unicode BMP (most common)
    if (encoding.platform_id == 3 && encoding.encoding_id == 1) {
      reader_.Seek(table_start + encoding.offset);

      uint16_t format = reader_.ReadUInt16();

      if (format != 4) {
        break;
      }

      uint16_t length = reader_.ReadUInt16();
      (void)length;

      cmap.format4.language = reader_.ReadUInt16();

      uint16_t seg_count_x2 = reader_.ReadUInt16();
      uint16_t seg_count = seg_count_x2 / 2;

      reader_.ReadUInt16();  // searchRange
      reader_.ReadUInt16();  // entrySelector
      reader_.ReadUInt16();  // rangeShift

      cmap.format4.end_codes.resize(seg_count);
      for (auto& value : cmap.format4.end_codes) {
        value = reader_.ReadUInt16();
      }

      reader_.ReadUInt16();  // reservedPad

      cmap.format4.start_codes.resize(seg_count);
      for (auto& value : cmap.format4.start_codes) {
        value = reader_.ReadUInt16();
      }

      cmap.format4.id_delta.resize(seg_count);
      for (auto& value : cmap.format4.id_delta) {
        value = reader_.ReadInt16();
      }

      cmap.format4.id_range_offset.resize(seg_count);
      for (auto& value : cmap.format4.id_range_offset) {
        value = reader_.ReadUInt16();
      }

      // Remaining bytes are glyphIdArray
      size_t bytes_read = (kBYTE * 2) +      // header
                          (seg_count * 2) +  // endCode
                          2 +                // reservedPad
                          (seg_count * 2) +  // startCode
                          (seg_count * 2) +  // idDelta
                          (seg_count * 2);   // idRangeOffset

      std::size_t remaining_bytes = length - bytes_read;
      std::size_t glyph_count = remaining_bytes / 2;

      cmap.format4.glyph_id_array.resize(glyph_count);

      for (auto& glyph : cmap.format4.glyph_id_array) {
        glyph = reader_.ReadUInt16();
      }

      break;
    }
  }

  return cmap;
}

auto TtfParser::LookupGlyph(char32_t c) const -> uint16_t {
  return cmap_.Lookup(static_cast<uint32_t>(c));
}

auto TtfParser::ReadHeader() -> GlyphHeader {
  GlyphHeader header{};

  header.number_of_contours = reader_.ReadInt16();
  header.x_min = reader_.ReadInt16();
  header.y_min = reader_.ReadInt16();
  header.x_max = reader_.ReadInt16();
  header.y_max = reader_.ReadInt16();

  return header;
}

auto TtfParser::ReadSimpleGlyph(const GlyphHeader& header) -> SimpleGlyph {
  SimpleGlyph glyph;
  glyph.header = header;

  glyph.end_pts_of_contours.resize(header.number_of_contours);

  for (auto& end_point : glyph.end_pts_of_contours) {
    end_point = reader_.ReadUInt16();
  }

  uint16_t instruction_length = reader_.ReadUInt16();

  glyph.instructions.resize(instruction_length);

  for (auto& byte : glyph.instructions) {
    byte = reader_.ReadUInt8();
  }

  uint16_t num_points = 0;

  if (!glyph.end_pts_of_contours.empty()) {
    num_points = glyph.end_pts_of_contours.back() + 1;
  }

  glyph.flags.reserve(num_points);

  while (glyph.flags.size() < num_points) {
    uint8_t flag = reader_.ReadUInt8();

    glyph.flags.push_back(flag);

    if ((flag & kRepeatFlag) != 0) {
      uint8_t repeat_count = reader_.ReadUInt8();

      for (uint8_t i = 0; i < repeat_count; ++i) {
        glyph.flags.push_back(flag);
      }
    }
  }

  glyph.x_coordinates.resize(num_points);

  int32_t x = 0;

  for (size_t i = 0; i < num_points; ++i) {
    uint8_t flag = glyph.flags.at(i);

    if ((flag & kXShortVector) != 0) {
      uint8_t dx = reader_.ReadUInt8();

      if ((flag & kXIsSameOrPositiveShortVector) != 0) {
        x += dx;
      } else {
        x -= dx;
      }
    } else if ((flag & kXIsSameOrPositiveShortVector) == 0) {
      x += reader_.ReadInt16();
    }

    glyph.x_coordinates.at(i) = static_cast<int16_t>(x);
  }

  glyph.y_coordinates.resize(num_points);

  int32_t y = 0;

  for (size_t i = 0; i < num_points; ++i) {
    uint8_t flag = glyph.flags.at(i);

    if ((flag & kYShortVector) != 0) {
      uint8_t dy = reader_.ReadUInt8();

      if ((flag & kYIsSameOrPositiveShortVector) != 0) {
        y += dy;
      } else {
        y -= dy;
      }
    } else if ((flag & kYIsSameOrPositiveShortVector) == 0) {
      y += reader_.ReadInt16();
    }

    glyph.y_coordinates.at(i) = static_cast<int16_t>(y);
  }

  return glyph;
}

auto TtfParser::ReadCompoundGlyph(const GlyphHeader& header) -> CompoundGlyph {
  CompoundGlyph glyph;
  glyph.header = header;

  constexpr uint16_t ARG_1_AND_2_ARE_WORDS = 0x0001;
  constexpr uint16_t ARGS_ARE_XY_VALUES = 0x0002;
  constexpr uint16_t WE_HAVE_A_SCALE = 0x0008;
  constexpr uint16_t MORE_COMPONENTS = 0x0020;
  constexpr uint16_t WE_HAVE_AN_X_AND_Y_SCALE = 0x0040;
  constexpr uint16_t WE_HAVE_A_TWO_BY_TWO = 0x0080;
  constexpr uint16_t WE_HAVE_INSTRUCTIONS = 0x0100;

  bool more = true;

  while (more) {
    GlyphComponent component;

    component.flags = reader_.ReadUInt16();
    component.glyph_index = reader_.ReadUInt16();

    if ((component.flags & ARG_1_AND_2_ARE_WORDS) != 0) {
      component.argument1 = reader_.ReadInt16();
      component.argument2 = reader_.ReadInt16();
    } else {
      component.argument1 = reader_.ReadUInt8();
      component.argument2 = reader_.ReadUInt8();
    }

    if ((component.flags & WE_HAVE_A_SCALE) != 0) {
      component.scale_x = reader_.ReadF2Dot14();
      component.scale_y = component.scale_x;
    } else if ((component.flags & WE_HAVE_AN_X_AND_Y_SCALE) != 0) {
      component.scale_x = reader_.ReadF2Dot14();
      component.scale_y = reader_.ReadF2Dot14();
    } else if ((component.flags & WE_HAVE_A_TWO_BY_TWO) != 0) {
      component.scale_x = reader_.ReadF2Dot14();
      component.scale01 = reader_.ReadF2Dot14();
      component.scale10 = reader_.ReadF2Dot14();
      component.scale_y = reader_.ReadF2Dot14();
    }

    glyph.components.push_back(component);

    more = ((component.flags & MORE_COMPONENTS) != 0);
  }

  if (!glyph.components.empty() && ((glyph.components.back().flags & WE_HAVE_INSTRUCTIONS) != 0)) {
    uint16_t instruction_length = reader_.ReadUInt16();

    glyph.instructions.resize(instruction_length);

    for (auto& byte : glyph.instructions) {
      byte = reader_.ReadUInt8();
    }
  }

  return glyph;
}

auto TtfParser::ReadGlyph(uint16_t glyph_index) -> Glyph {
  LocaTable loca = ReadLocaTable();

  uint32_t start = loca.GlyphOffset(glyph_index);
  uint32_t end = loca.GlyphOffset(glyph_index + 1);

  if (start == end) {
    return Glyph{};
  }

  const TableRecord* glyf = font_.GetGlyf();

  if (glyf == nullptr) {
    throw std::runtime_error("Missing glyf table.");
  }

  reader_.Seek(glyf->offset + start);

  GlyphHeader header = ReadHeader();
  Glyph glyph;

  if (header.number_of_contours >= 0) {
    glyph.data = ReadSimpleGlyph(header);
  } else {
    glyph.data = ReadCompoundGlyph(header);
  }

  return glyph;
}

}  // namespace glyph

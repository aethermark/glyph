#pragma once

#include <cstdint>
#include <printer/table.hpp>
#include <string>
#include <tuple>

namespace glyph {

struct TableDirectory {
public:
  uint32_t sfnt_version;
  uint16_t num_tables;
  uint16_t search_range;
  uint16_t entry_selector;
  uint16_t range_shift;

  static constexpr auto Name() -> std::string_view {
    return "TableDirectory";
  }

  static constexpr auto Fields() {
    using Self = TableDirectory;

    return std::make_tuple(
        printer::MakeField(
            "sfnt_version",
            [](const Self& self) -> std::string { return printer::Hex(self.sfnt_version); },
            printer::Alignment::kRight),
        printer::MakeMemberField("num_tables", &Self::num_tables, printer::Alignment::kRight),
        printer::MakeMemberField("search_range", &Self::search_range, printer::Alignment::kRight),
        printer::MakeMemberField("entry_selector", &Self::entry_selector,
                                 printer::Alignment::kRight),
        printer::MakeMemberField("range_shift", &Self::range_shift, printer::Alignment::kRight));
  }
};

struct TableRecord {
public:
  std::string tag;
  uint32_t checksum;
  uint32_t offset;
  uint32_t length;

  static constexpr auto Name() -> std::string_view {
    return "TableRecord";
  }

  static constexpr auto Fields() {
    using Self = TableRecord;

    return std::make_tuple(
        printer::MakeMemberField("tag", &Self::tag, printer::Alignment::kLeft),
        printer::MakeField(
            "checksum", [](const Self& self) -> std::string { return printer::Hex(self.checksum); },
            printer::Alignment::kRight),
        printer::MakeMemberField("offset", &Self::offset, printer::Alignment::kRight),
        printer::MakeMemberField("length", &Self::length, printer::Alignment::kRight));
  }
};

struct HeadTable {
public:
  uint16_t major_version;
  uint16_t minor_version;
  float font_revision;
  uint32_t checksum_adjustment;
  uint32_t magic_number;
  uint16_t flags;
  uint16_t units_per_em;

  // TODO(MukulWaval): longdatetime created;
  // TODO(MukulWaval): longdatetime modified;

  int16_t x_min;
  int16_t y_min;
  int16_t x_max;
  int16_t y_max;
  uint16_t mac_style;
  uint16_t lowest_rec_ppem;
  int16_t font_direction_hint;
  int16_t index_to_loc_format;
  int16_t glyph_data_format;

  static constexpr auto Name() -> std::string_view {
    return "HeadTable";
  }

  static constexpr auto Fields() {
    using Self = HeadTable;

    return std::make_tuple(
        printer::MakeField(
            "major_version",
            [](const Self& self) -> std::string { return printer::Hex(self.major_version); }),
        printer::MakeField(
            "minor_version",
            [](const Self& self) -> std::string { return printer::Hex(self.minor_version); }),
        printer::MakeField(
            "font_revision",
            [](const Self& self) -> std::string { return printer::ToCell(self.font_revision); }),
        printer::MakeField(
            "checksum_adjustment",
            [](const Self& self) -> std::string { return printer::Hex(self.checksum_adjustment); }),
        printer::MakeField(
            "magic_number",
            [](const Self& self) -> std::string { return printer::Hex(self.magic_number); }),
        printer::MakeField(
            "flags", [](const Self& self) -> std::string { return printer::Hex(self.flags); }),
        printer::MakeField(
            "units_per_em",
            [](const Self& self) -> std::string { return printer::ToCell(self.units_per_em); }),
        printer::MakeField(
            "x_min", [](const Self& self) -> std::string { return printer::ToCell(self.x_min); }),
        printer::MakeField(
            "y_min", [](const Self& self) -> std::string { return printer::ToCell(self.y_min); }),
        printer::MakeField(
            "x_max", [](const Self& self) -> std::string { return printer::ToCell(self.x_max); }),
        printer::MakeField(
            "y_max", [](const Self& self) -> std::string { return printer::ToCell(self.y_max); }),
        printer::MakeField(
            "mac_style",
            [](const Self& self) -> std::string { return printer::Hex(self.mac_style); }),
        printer::MakeField(
            "lowest_rec_ppem",
            [](const Self& self) -> std::string { return printer::ToCell(self.lowest_rec_ppem); }),
        printer::MakeField("font_direction_hint",
                           [](const Self& self) -> std::string {
                             return printer::ToCell(self.font_direction_hint);
                           }),
        printer::MakeField("index_to_loc_format",
                           [](const Self& self) -> std::string {
                             return printer::ToCell(self.index_to_loc_format);
                           }),
        printer::MakeField("glyph_data_format", [](const Self& self) -> std::string {
          return printer::ToCell(self.glyph_data_format);
        }));
  }
};

struct MaxpTable {
public:
  uint32_t version;
  uint16_t num_glyphs;

  uint16_t max_points;
  uint16_t max_contours;
  uint16_t max_composite_points;
  uint16_t max_composite_contours;
  uint16_t max_zones;
  uint16_t max_twilight_points;
  uint16_t max_storage;
  uint16_t max_function_defs;
  uint16_t max_instruction_defs;
  uint16_t max_stack_elements;
  uint16_t max_size_of_instructions;
  uint16_t max_component_elements;
  uint16_t max_component_depth;

  static constexpr auto Name() -> std::string_view {
    return "MaxpTable";
  }

  static constexpr auto Fields() {
    using Self = MaxpTable;

    return std::make_tuple(
        printer::MakeField(
            "version", [](const Self& self) -> std::string { return printer::Hex(self.version); }),
        printer::MakeField(
            "num_glyphs",
            [](const Self& self) -> std::string { return printer::Hex(self.num_glyphs); }),
        printer::MakeField(
            "max_points",
            [](const Self& self) -> std::string { return printer::Hex(self.max_points); }),
        printer::MakeField(
            "max_contours",
            [](const Self& self) -> std::string { return printer::Hex(self.max_contours); }),
        printer::MakeField("max_composite_points",
                           [](const Self& self) -> std::string {
                             return printer::Hex(self.max_composite_points);
                           }),
        printer::MakeField("max_composite_contours",
                           [](const Self& self) -> std::string {
                             return printer::Hex(self.max_composite_contours);
                           }),
        printer::MakeField(
            "max_zones",
            [](const Self& self) -> std::string { return printer::Hex(self.max_zones); }),
        printer::MakeField(
            "max_twilight_points",
            [](const Self& self) -> std::string { return printer::Hex(self.max_twilight_points); }),
        printer::MakeField(
            "max_storage",
            [](const Self& self) -> std::string { return printer::Hex(self.max_storage); }),
        printer::MakeField(
            "max_function_defs",
            [](const Self& self) -> std::string { return printer::Hex(self.max_function_defs); }),
        printer::MakeField("max_instruction_defs",
                           [](const Self& self) -> std::string {
                             return printer::Hex(self.max_instruction_defs);
                           }),
        printer::MakeField(
            "max_stack_elements",
            [](const Self& self) -> std::string { return printer::Hex(self.max_stack_elements); }),
        printer::MakeField("max_size_of_instructions",
                           [](const Self& self) -> std::string {
                             return printer::Hex(self.max_size_of_instructions);
                           }),
        printer::MakeField("max_component_elements",
                           [](const Self& self) -> std::string {
                             return printer::Hex(self.max_component_elements);
                           }),
        printer::MakeField("max_component_depth", [](const Self& self) -> std::string {
          return printer::Hex(self.max_component_depth);
        }));
  }
};

struct LocaTable {
public:
  std::vector<uint32_t> offsets;

  [[nodiscard]]
  auto GlyphOffset(uint16_t glyph_id) const -> uint32_t {
    return offsets.at(glyph_id);
  }

  [[nodiscard]]
  auto GlyphLength(uint16_t glyph_id) const -> uint32_t {
    return offsets.at(glyph_id + 1) - offsets.at(glyph_id);
  }

  static constexpr auto Name() -> std::string_view {
    return "LocaTable";
  }

  static constexpr auto Fields() {
    using Self = LocaTable;

    return std::make_tuple(printer::MakeField("glyph_count",
                                              [](const Self& self) -> std::string {
                                                return std::to_string(self.offsets.size() - 1);
                                              }),
                           printer::MakeField("table_size", [](const Self& self) -> std::string {
                             return std::to_string(self.offsets.size());
                           }));
  }
};

}  // namespace glyph

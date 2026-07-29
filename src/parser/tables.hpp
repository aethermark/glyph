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

}  // namespace glyph

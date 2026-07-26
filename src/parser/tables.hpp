#pragma once

#include <cstdint>
#include <string>
#include <tuple>

#include "printer.hpp"

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
        MakeField(
            "sfnt_version", [](const Self& self) -> std::string { return Hex(self.sfnt_version); },
            Alignment::kRight),
        MakeMemberField("num_tables", &Self::num_tables, Alignment::kRight),
        MakeMemberField("search_range", &Self::search_range, Alignment::kRight),
        MakeMemberField("entry_selector", &Self::entry_selector, Alignment::kRight),
        MakeMemberField("range_shift", &Self::range_shift, Alignment::kRight));
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
        MakeMemberField("tag", &Self::tag, Alignment::kLeft),
        MakeField(
            "checksum", [](const Self& self) -> std::string { return Hex(self.checksum); },
            Alignment::kRight),
        MakeMemberField("offset", &Self::offset, Alignment::kRight),
        MakeMemberField("length", &Self::length, Alignment::kRight));
  }
};

}  // namespace glyph

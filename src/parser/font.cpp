#include "parser/font.hpp"

#include <string>
#include <utility>

#include "parser/tables.hpp"

namespace glyph {

auto glyph::Font::FindTable(std::string_view tag) const -> const TableRecord* {
  for (auto const& table : tables_) {
    if (table.tag == tag) {
      return &table;
    }
  }

  return nullptr;
}

auto glyph::Font::GetHead() const -> const TableRecord* {
  return FindTable("head");
}

auto glyph::Font::GetMaxp() const -> const TableRecord* {
  return FindTable("maxp");
}

auto glyph::Font::GetLoca() const -> const TableRecord* {
  return FindTable("loca");
}

auto glyph::Font::GetCmap() const -> const TableRecord* {
  return FindTable("cmap");
}

auto glyph::Font::GetGlyf() const -> const TableRecord* {
  return FindTable("glyf");
}

auto glyph::Font::SetDirectory(const TableDirectory& directory) -> void {
  directory_ = directory;
}

auto glyph::Font::SetTables(std::vector<TableRecord>&& tables) -> void {
  tables_ = std::move(tables);
}

auto glyph::Font::GetDirectory() const -> const TableDirectory* {
  return &directory_;
}

auto glyph::Font::GetTables() const -> const std::vector<TableRecord>* {
  return &tables_;
}

}  // namespace glyph

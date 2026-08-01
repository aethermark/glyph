#pragma once

#include <string_view>
#include <vector>

#include "parser/tables.hpp"

namespace glyph {

class Font {
public:
  Font() = default;

  [[nodiscard]] auto FindTable(std::string_view tag) const -> const TableRecord*;

  [[nodiscard]] auto GetHead() const -> const TableRecord*;
  [[nodiscard]] auto GetMaxp() const -> const TableRecord*;
  [[nodiscard]] auto GetLoca() const -> const TableRecord*;
  [[nodiscard]] auto GetGlyf() const -> const TableRecord*;

  auto SetDirectory(const TableDirectory& directory) -> void;
  auto SetTables(std::vector<TableRecord>&& tables) -> void;

  [[nodiscard]] auto GetDirectory() const -> const TableDirectory*;
  [[nodiscard]] auto GetTables() const -> const std::vector<TableRecord>*;

private:
  TableDirectory directory_{};
  std::vector<TableRecord> tables_;
};

}  // namespace glyph

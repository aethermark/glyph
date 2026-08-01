#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <parser/tables.hpp>
#include <string>
#include <vector>

#include "parser/font.hpp"

namespace glyph {

/// @brief Loads the given TTF file.
class BinaryReader {
public:
  auto LoadData(const std::filesystem::path& path) -> bool;
  auto ReadUInt8() -> uint8_t;
  auto ReadUInt16() -> uint16_t;
  auto ReadUInt32() -> uint32_t;
  auto ReadInt16() -> int16_t;
  auto ReadInt32() -> int32_t;
  auto ReadFWord() -> int16_t;
  auto ReadUFWord() -> uint16_t;
  auto ReadOffset16() -> uint16_t;
  auto ReadOffset32() -> uint32_t;
  auto ReadF2Dot14() -> float;
  auto ReadFixed() -> float;
  auto ReadTag() -> std::string;
  // TODO(MukulWaval): implement ReadDate()
  auto ReadTableDirectory() -> TableDirectory;
  auto ReadTableRecord() -> TableRecord;
  auto ReadFont(const std::filesystem::path& path) -> Font;
  [[nodiscard]] auto GetPosition() const -> size_t;
  [[nodiscard]] auto GetData() const -> const std::vector<uint8_t>&;

  auto Seek(std::size_t position) -> void;
  auto Skip(std::size_t bytes) -> void;

private:
  size_t position_{};
  uintmax_t size_{};
  std::vector<uint8_t> data_;
};

}  // namespace glyph

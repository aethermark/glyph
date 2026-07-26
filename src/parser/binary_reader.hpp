#pragma once

#include <cstdint>
#include <filesystem>
#include <parser/tables.hpp>
#include <string>
#include <vector>

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
  auto GetPosition() -> int;
  auto SetPosition() -> void;
  [[nodiscard]]
  auto GetData() const -> const std::vector<uint8_t>&;

private:
  int position_;
  uintmax_t size_;
  std::vector<uint8_t> data_;
  static constexpr uint8_t kSHIFT_BYTE = 8;
};

}  // namespace glyph
   // namespace glyph

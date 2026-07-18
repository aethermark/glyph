#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace glyph {

/// @brief Loads the given TTF file.
class BinaryReader {
public:
  auto LoadData(const std::filesystem::path& path) -> bool;
  auto ReadUInt8() -> uint8_t;
  auto ReadUInt16() -> uint16_t;
  auto ReadUint32() -> uint32_t;
  auto ReadInt16() -> int16_t;
  auto ReadInt32() -> int32_t;
  auto ReadFWord() -> int16_t;
  auto ReadUFWord() -> uint16_t;
  auto ReadOffset16() -> uint16_t;
  auto ReadOffset32() -> uint32_t;
  auto ReadF2Dot14() -> double;
  auto ReadFixed() -> double;
  auto ReadString() -> std::string;
  // TODO(MukulWaval): implement ReadDate()
  auto GetPosition() -> int;
  auto SetPosition() -> void;
  [[nodiscard]]
  auto GetData() const -> const std::vector<uint8_t>&;

private:
  int position_;
  std::vector<uint8_t> data_;
};

}  // namespace glyph

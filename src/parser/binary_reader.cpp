#include "parser/binary_reader.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <utility>

#include "parser/constants.hpp"
#include "parser/tables.hpp"

auto glyph::BinaryReader::LoadData(const std::filesystem::path& path) -> bool {
  position_ = 0;
  std::ifstream file(path, std::ios::binary);

  if (!file) {
    return false;
  }

  size_ = std::filesystem::file_size(path);

  data_.resize(size_);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  file.read(reinterpret_cast<char*>(data_.data()), static_cast<std::streamsize>(size_));

  if (file.gcount() != static_cast<std::streamsize>(size_)) {
    data_.clear();
    return false;
  }

  return true;
}

auto glyph::BinaryReader::ReadUInt8() -> uint8_t {
  return data_.at(position_++);
}

auto glyph::BinaryReader::ReadUInt16() -> uint16_t {
  uint16_t data = static_cast<uint16_t>(data_.at(position_++)) << kBYTE;
  data |= static_cast<uint16_t>(data_.at(position_++));
  return data;
}

auto glyph::BinaryReader::ReadUInt32() -> uint32_t {
  uint32_t data = static_cast<uint32_t>(data_.at(position_++)) << 3 * kBYTE;
  data |= static_cast<uint32_t>(data_.at(position_++)) << 2 * kBYTE;
  data |= static_cast<uint32_t>(data_.at(position_++)) << kBYTE;
  data |= static_cast<uint32_t>(data_.at(position_++));
  return data;
}

auto glyph::BinaryReader::ReadInt16() -> int16_t {
  uint16_t data = static_cast<uint16_t>(data_.at(position_++)) << kBYTE;
  data |= static_cast<uint16_t>(data_.at(position_++));
  return static_cast<int16_t>(data);
}

auto glyph::BinaryReader::ReadInt32() -> int32_t {
  uint32_t data = static_cast<uint32_t>(data_.at(position_++)) << 3 * kBYTE;
  data |= static_cast<uint32_t>(data_.at(position_++)) << 2 * kBYTE;
  data |= static_cast<uint32_t>(data_.at(position_++)) << kBYTE;
  data |= static_cast<uint32_t>(data_.at(position_++));
  return static_cast<int32_t>(data);
}

auto glyph::BinaryReader::ReadFWord() -> int16_t {
  return ReadInt16();
}

auto glyph::BinaryReader::ReadUFWord() -> uint16_t {
  return ReadUInt16();
}

auto glyph::BinaryReader::ReadOffset16() -> uint16_t {
  return ReadUInt16();
}

auto glyph::BinaryReader::ReadOffset32() -> uint32_t {
  return ReadUInt32();
}

auto glyph::BinaryReader::ReadF2Dot14() -> float {
  constexpr float scale = 16384.0F;  // 2^14
  return static_cast<float>(ReadInt16()) / scale;
}

auto glyph::BinaryReader::ReadFixed() -> float {
  constexpr float scale = 65536.0F;  // 2^16
  return static_cast<float>(ReadInt32()) / scale;
}

auto glyph::BinaryReader::ReadTag() -> std::string {
  std::string tag;
  tag.reserve(4);

  tag.push_back(static_cast<char>(ReadUInt8()));
  tag.push_back(static_cast<char>(ReadUInt8()));
  tag.push_back(static_cast<char>(ReadUInt8()));
  tag.push_back(static_cast<char>(ReadUInt8()));

  return tag;
}

auto glyph::BinaryReader::ReadTableDirectory() -> TableDirectory {
  return {
      .sfnt_version = ReadUInt32(),
      .num_tables = ReadUInt16(),
      .search_range = ReadUInt16(),
      .entry_selector = ReadUInt16(),
      .range_shift = ReadUInt16(),
  };
}

auto glyph::BinaryReader::ReadTableRecord() -> TableRecord {
  return {
      .tag = ReadTag(),
      .checksum = ReadUInt32(),
      .offset = ReadOffset32(),
      .length = ReadUInt32(),
  };
}

auto glyph::BinaryReader::ReadFont(const std::filesystem::path& path) -> Font {
  Font font;

  if (!LoadData(path)) {
    return font;
  }

  const TableDirectory directory = ReadTableDirectory();

  std::vector<TableRecord> tables;
  tables.reserve(directory.num_tables);

  for (uint16_t i = 0; i < directory.num_tables; ++i) {
    tables.push_back(ReadTableRecord());
  }

  font.SetDirectory(directory);
  font.SetTables(std::move(tables));

  return font;
}

auto glyph::BinaryReader::GetPosition() const -> size_t {
  return position_;
}

auto glyph::BinaryReader::GetData() const -> const std::vector<uint8_t>& {
  return data_;
}

auto glyph::BinaryReader::Seek(std::size_t position) -> void {
  if (position > data_.size()) {
    throw std::out_of_range("BinaryReader::Seek");
  }

  position_ = position;
}

auto glyph::BinaryReader::Skip(std::size_t bytes) -> void {
  Seek(position_ + bytes);
}

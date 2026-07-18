#include "parser/binary_reader.hpp"

#include <fstream>

auto glyph::BinaryReader::LoadData(const std::filesystem::path& path) -> bool {
  std::ifstream file(path, std::ios::binary);

  if (!file) {
    return false;
  }

  const auto size = std::filesystem::file_size(path);

  data_.resize(size);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  file.read(reinterpret_cast<char*>(data_.data()), static_cast<std::streamsize>(size));

  if (file.gcount() != static_cast<std::streamsize>(size)) {
    data_.clear();
    return false;
  }

  return true;
}

auto glyph::BinaryReader::GetData() const -> const std::vector<uint8_t>& {
  return data_;
}

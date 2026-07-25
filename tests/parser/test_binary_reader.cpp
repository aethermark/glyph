#include <stdint.h>

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <vector>

#include "parser/binary_reader.hpp"

TEST_CASE("BinaryReader loads a binary file", "[binary_reader]") {
  const std::filesystem::path path = std::filesystem::temp_directory_path() / "test.bin";

  {
    std::ofstream file(path, std::ios::binary);

    const std::vector<uint8_t> expected = {
        0xDE, 0xAD, 0xBE, 0xEF, 0x12, 0x34,
    };

    file.write(reinterpret_cast<const char*>(expected.data()),
               static_cast<std::streamsize>(expected.size()));
  }

  glyph::BinaryReader reader;

  REQUIRE(reader.LoadData(path));

  REQUIRE(reader.GetData().size() == 6);

  REQUIRE(reader.GetData()[0] == 0xDE);
  REQUIRE(reader.GetData()[1] == 0xAD);
  REQUIRE(reader.GetData()[2] == 0xBE);
  REQUIRE(reader.GetData()[3] == 0xEF);
  REQUIRE(reader.GetData()[4] == 0x12);
  REQUIRE(reader.GetData()[5] == 0x34);

  std::filesystem::remove(path);
}

TEST_CASE("BinaryReader fails for a nonexistent file", "[binary_reader]") {
  glyph::BinaryReader reader;

  REQUIRE_FALSE(reader.LoadData("this_file_should_not_exist.bin"));
}

TEST_CASE("ReadUIntX_t return correct values", "[binary_reader]") {
  const std::filesystem::path path = std::filesystem::temp_directory_path() / "test.bin";

  {
    std::ofstream file(path, std::ios::binary);

    const std::vector<uint8_t> expected = {0xDE, 0xAD, 0xBE, 0xEF, 0x12, 0x34, 0x38};

    file.write(reinterpret_cast<const char*>(expected.data()),
               static_cast<std::streamsize>(expected.size()));
  }

  glyph::BinaryReader reader;
  reader.LoadData(path);

  REQUIRE(reader.ReadUInt8() == 0xDE);
  REQUIRE(reader.ReadUInt16() == 0xADBE);
  REQUIRE(reader.ReadUInt32() == 0xEF123438);
}

TEST_CASE("ReadIntX_t return correct values", "[binary_reader]") {
  const std::filesystem::path path = std::filesystem::temp_directory_path() / "test.bin";

  {
    std::ofstream file(path, std::ios::binary);

    const std::vector<uint8_t> expected = {0xAD, 0xBE, 0xEF, 0x12, 0x34, 0x38};

    file.write(reinterpret_cast<const char*>(expected.data()),
               static_cast<std::streamsize>(expected.size()));
  }

  glyph::BinaryReader reader;
  reader.LoadData(path);

  REQUIRE(reader.ReadInt16() == static_cast<int16_t>(0xADBE));
  REQUIRE(reader.ReadInt32() == static_cast<int32_t>(0xEF123438));
}

TEST_CASE("ReadU?IntX_t fails to read nonexistant data", "[binary_reader]") {
  const std::filesystem::path path = std::filesystem::temp_directory_path() / "test.bin";
  {
    std::ofstream file(path, std::ios::binary);

    const std::vector<uint8_t> expected = {0xDE};

    file.write(reinterpret_cast<const char*>(expected.data()),
               static_cast<std::streamsize>(expected.size()));
  }

  glyph::BinaryReader reader;
  reader.LoadData(path);
  auto _ = reader.ReadUInt8();

  REQUIRE_THROWS(reader.ReadUInt8());
  REQUIRE_THROWS(reader.ReadUInt16());
  REQUIRE_THROWS(reader.ReadUInt32());
  REQUIRE_THROWS(reader.ReadInt16());
  REQUIRE_THROWS(reader.ReadInt32());
}

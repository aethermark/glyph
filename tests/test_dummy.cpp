#include <catch2/catch_test_macros.hpp>
#include <glyph/dummy.hpp>

TEST_CASE("add() adds two positive numbers", "[math]") {
  REQUIRE(glyph::add(2, 3) == 5);
}

TEST_CASE("add() handles zero", "[math]") {
  REQUIRE(glyph::add(5, 0) == 5);
  REQUIRE(glyph::add(0, 5) == 5);
}

TEST_CASE("add() handles negative numbers", "[math]") {
  REQUIRE(glyph::add(-2, 3) == 1);
  REQUIRE(glyph::add(-2, -3) == -5);
}

TEST_CASE("add() is commutative", "[math]") {
  REQUIRE(glyph::add(17, 42) == glyph::add(42, 17));
}

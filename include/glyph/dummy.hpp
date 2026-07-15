#pragma once

#include <cstdint>

namespace glyph {

/// Adds two integers.
///
/// @param lhs The left-hand operand.
/// @param rhd The right-hand operand.
/// @return The sum of `lhs` and `rhs`.
[[nodiscard]]
auto add(int lhs, int rhs) -> int;

}  // namespace glyph

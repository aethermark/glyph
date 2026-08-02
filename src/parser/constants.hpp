#pragma once

#include <cstdint>

namespace glyph {

inline constexpr uint8_t kBYTE = 8;

inline constexpr uint8_t kOnCurvePoint = 0x01;
inline constexpr uint8_t kXShortVector = 0x02;
inline constexpr uint8_t kYShortVector = 0x04;
inline constexpr uint8_t kRepeatFlag = 0x08;
inline constexpr uint8_t kXIsSameOrPositiveShortVector = 0x10;
inline constexpr uint8_t kYIsSameOrPositiveShortVector = 0x20;

}  // namespace glyph

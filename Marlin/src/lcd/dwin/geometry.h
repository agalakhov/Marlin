/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * DWIN by Creality3D
 * Rewrite and Extui Port by Jacob Myers
 * Refactor by Alexey Galakhov
 */

#include "../../inc/MarlinConfigPre.h"
#include <cstdint>
#include <cstring>
#include "dwin.h"

/* Geometry of the Creality display */

namespace Creality {

  struct Point {
    uint16_t x;
    uint16_t y;
  };

  struct Rectangle {
    Point tl; // top left
    Point br; // bottom right
  public:
    constexpr Rectangle Bottom(uint16_t height) const {
      return { { tl.x, static_cast<uint16_t>(br.y - height) }, br };
    }
  };

  struct Dimensions {
    uint16_t w;
    uint16_t h;
  public:
    constexpr Rectangle AtPoint(Point p) const {
      return { p, { static_cast<uint16_t>(p.x + w), static_cast<uint16_t>(p.y + h) } };
    }
  };

  struct Grid {
    Point origin;
    Dimensions step;
  public:
    constexpr Point At(uint8_t col, uint8_t row) const {
      return { static_cast<uint16_t>(origin.x + step.w * col), static_cast<uint16_t>(origin.y + step.h * row) };
    }
  };

  class BaseGeometry {
  protected:
    static constexpr uint16_t screenLongSide = DWIN_HEIGHT;
    static constexpr uint16_t screenShortSide = DWIN_WIDTH;
  public:
    static constexpr DWIN::Font titleFont = DWIN::Font::font10x20;
    static constexpr DWIN::Font statusFont = DWIN::Font::font10x20;
    static constexpr DWIN::Font menuFont = DWIN::Font::font8x16;

    static constexpr Dimensions mainMenuItemSize = { 109, 99 };
    static constexpr Dimensions printMenuItemSize = { 79, 99 };
    static constexpr uint16_t iconMenuTextHeight = 40;

  public:
    static constexpr Dimensions Font_CharSize(DWIN::Font font);
    static constexpr Dimensions Font_TextDimensions(DWIN::Font font, const char * text);
    static constexpr Point Text_Inscribe(const Rectangle& rect, DWIN::Font font, const char * text);
  };

  class PortraitGeometry : public BaseGeometry {
  public:
    static constexpr Dimensions screen = { screenShortSide, screenLongSide };
    static constexpr uint16_t titleHeight = 30;
    static constexpr Grid mainMenuGrid = { { 17, 130 }, { 128, 116 } };
    static constexpr Grid printMenuGrid = { { 8, 252 }, { 88 , 0 } };
    static constexpr Point logoPos = { 71, 72 };
    static constexpr Rectangle printProgressBarPos = { { 15, 93 }, { 256, 113 } };
    static constexpr Point printPercentPos = { 109, 133 };
    static constexpr Point printElapsedTimePos = { 42, 187 };
    static constexpr Point printRemainingTimePos = { 176, 187 };
  };

  // These functions must be defined in header file since they all are constexpr.

  constexpr Dimensions BaseGeometry::Font_CharSize(DWIN::Font font) {
    switch (font) {
      case DWIN::Font::font5x12:
        return { 5, 12 };
      case DWIN::Font::font8x16:
        return { 8, 16 };
      case DWIN::Font::font10x20:
        return { 10, 20 };
      case DWIN::Font::font12x24:
        return { 12, 24 };
      case DWIN::Font::font14x28:
        return { 14, 28 };
      case DWIN::Font::font16x32:
        return { 16, 32 };
      case DWIN::Font::font20x40:
        return { 20, 40 };
      case DWIN::Font::font24x48:
        return { 24, 48 };
      case DWIN::Font::font28x56:
        return { 28, 56 };
      case DWIN::Font::font32x64:
        return { 32, 64 };
    }
    return { 0, 0 }; // unreachable
  }

  constexpr Dimensions BaseGeometry::Font_TextDimensions(DWIN::Font font, const char * text) {
    const auto d = Font_CharSize(font);
    return { static_cast<uint16_t>(d.w * strlen(text)), d.h };
  }

  static constexpr uint16_t Center(uint16_t s, uint16_t a, uint16_t b) {
    return a + (b - a - s) / 2;
  }

  constexpr Point BaseGeometry::Text_Inscribe(const Rectangle& rect, DWIN::Font font, const char * text) {
    const auto d = Font_TextDimensions(font, text);
    return { Center(d.w, rect.tl.x, rect.br.x), Center(d.h, rect.tl.y, rect.br.y) };
  }

  using Geometry = PortraitGeometry;
} // namespace Creality

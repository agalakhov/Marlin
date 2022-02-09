/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// Extended and default UI Colors
#define RGB(R,G,B)  (R << 11) | (G << 5) | (B) // R,B: 0..31; G: 0..63
#define GetRColor(color) ((color >> 11) & 0x1F)
#define GetGColor(color) ((color >>  5) & 0x3F)
#define GetBColor(color) ((color >>  0) & 0x1F)

class Color {
  public:
    // red, blue: 0..31 (0x1f); green: 0..63 (0x3f)
    inline constexpr Color(uint8_t red, uint8_t green, uint8_t blue)
      : value(((red & 0x1f) << 11) | ((green & 0x3f) << 5) | (blue & 0x1f))
    { }
    inline constexpr uint8_t red() const {
      return (value >> 11) & 0x1f;
    }
    inline constexpr uint8_t green() const {
      return (value >> 5) & 0x3f;
    }
    inline constexpr uint8_t blue() const {
      return (value >> 0) & 0x1f;
    }
    inline constexpr uint16_t bits() const { return value; }
  private:
    uint16_t value;
};

struct StdColor {
  inline static const constexpr Color White  = { 0x1f, 0x3f, 0x1f };
  inline static const constexpr Color Yellow = { 0x1f, 0x3f, 0x00 };
  inline static const constexpr Color Red    = { 0x1f, 0x00, 0x00 };
};

static const constexpr Color Color_Error_Red = { 0x16, 0x00, 0x00 }; // Error!
static const constexpr Color Color_Bg_Red = { 0x1e, 0x00, 0x0f }; // Red background color
static const constexpr Color Color_Bg_Window = { 0x06, 0x0f, 0x08 }; // Popup background color
static const constexpr Color Color_Bg_Blue = { 0x02, 0x09, 0x05 }; // Dark blue background color
static const constexpr Color Color_Bg_Black = { 0x01, 0x02, 0x01 }; // Black background color
static const constexpr Color Color_IconBlue = { 0x08, 0x2f, 0x1a }; // Lighter blue that matches icons/accents
static const constexpr Color Popup_Text_Color = { 0x1a, 0x35, 0x1a }; // Popup font background color
static const constexpr Color Line_Color = { 0x07, 0x13, 0x0a }; // Split line color
static const constexpr Color Rectangle_Color = { 0x1d, 0x31, 0x0f }; // Blue square cursor color
static const constexpr Color Percent_Color = { 0x1f, 0x31, 0x09 }; // Percentage color
static const constexpr Color BarFill_Color = { 0x02, 0x07, 0x04 }; // Fill color of progress bar
static const constexpr Color Select_Color = { 0x06, 0x1d, 0x1b }; // Selected color

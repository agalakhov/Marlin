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

class Font {
  public:
  /**
   * 3-.0：The font size, 0x00-0x09, corresponds to the font size below:
   * 0x00=6*12   0x01=8*16   0x02=10*20  0x03=12*24  0x04=14*28
   * 0x05=16*32  0x06=20*40  0x07=24*48  0x08=28*56  0x09=32*64
   */
  enum Index {
    f6x12  = 0x00,
    f8x16  = 0x01,
    f10x20 = 0x02,
    f12x24 = 0x03,
    f14x28 = 0x04,
    f16x32 = 0x05,
    f20x40 = 0x06,
    f24x48 = 0x07,
    f28x56 = 0x08,
    f32x64 = 0x09,
  };

  public:
    constexpr inline uint8_t width() const {
      switch (index) {
        case f6x12 : return 6;
        case f8x16 : return 8;
        case f10x20: return 10;
        case f12x24: return 12;
        case f14x28: return 14;
        case f16x32: return 16;
        case f20x40: return 20;
        case f24x48: return 24;
        case f28x56: return 28;
        case f32x64: return 32;
        default: return 0; // unreachable
      }
    }

    constexpr inline uint8_t height() const {
      switch (index) {
        case f6x12 : return 12;
        case f8x16 : return 16;
        case f10x20: return 20;
        case f12x24: return 24;
        case f14x28: return 28;
        case f16x32: return 32;
        case f20x40: return 40;
        case f24x48: return 48;
        case f28x56: return 56;
        case f32x64: return 64;
        default: return 0; // unreachable
      }
    }

    constexpr inline uint8_t descent() const {
      switch (index) {
        case f6x12 : return 2;
        case f8x16 : return 3;
        case f10x20: return 4;
        case f12x24: return 5;
        case f14x28: return 6;
        case f16x32: return 6;
        case f20x40: return 8;
        case f24x48: return 10;
        case f28x56: return 12;
        case f32x64: return 14;
        default: return 0; // unreachable
      }
    }

    constexpr inline uint8_t ascent() const {
      return height() - descent();
    }

    constexpr inline Font(const Font::Index idx) : index(idx) { }
    constexpr inline uint8_t idx() const { return index; }
    constexpr inline bool operator== (const Font& rhs) const {
      return index == rhs.index;
    }
    constexpr inline bool operator!= (const Font& rhs) const {
        return !operator==(rhs);
    }
  private:
    uint8_t index;
};

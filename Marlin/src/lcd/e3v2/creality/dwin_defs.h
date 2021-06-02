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

#include "../../../inc/MarlinConfigPre.h"
#include <cstdint>

/* Constants for the DWIN display */

namespace DWIN {

  // FIXME Create a dedicated type for these constants
  constexpr uint8_t ICON = 0x09;

  enum class Icon: uint8_t {
    // Stock firmware icons
    LOGO               = 0,
    Print_0            = 1,
    Print_1            = 2,
    Prepare_0          = 3,
    Prepare_1          = 4,
    Control_0          = 5,
    Control_1          = 6,
    Leveling_0         = 7,
    Leveling_1         = 8,
    HotendTemp         = 9,
    BedTemp            = 10,
    Speed              = 11,
    Zoffset            = 12,
    Back               = 13,
    File               = 14,
    PrintTime          = 15,
    RemainTime         = 16,
    Setup_0            = 17,
    Setup_1            = 18,
    Pause_0            = 19,
    Pause_1            = 20,
    Continue_0         = 21,
    Continue_1         = 22,
    Stop_0             = 23,
    Stop_1             = 24,
    Bar                = 25,
    More               = 26,

    Axis               = 27,
    CloseMotor         = 28,
    Homing             = 29,
    SetHome            = 30,
    PLAPreheat         = 31,
    ABSPreheat         = 32,
    Cool               = 33,
    Language           = 34,

    MoveX              = 35,
    MoveY              = 36,
    MoveZ              = 37,
    Extruder           = 38,

    Temperature        = 40,
    Motion             = 41,
    WriteEEPROM        = 42,
    ReadEEPROM         = 43,
    ResumeEEPROM       = 44,
    Info               = 45,

    SetEndTemp         = 46,
    SetBedTemp         = 47,
    FanSpeed           = 48,
    SetPLAPreheat      = 49,
    SetABSPreheat      = 50,

    MaxSpeed           = 51,
    MaxAccelerated     = 52,
    MaxJerk            = 53,
    Step               = 54,
    PrintSize          = 55,
    Version            = 56,
    Contact            = 57,
    StockConfiguraton  = 58,
    MaxSpeedX          = 59,
    MaxSpeedY          = 60,
    MaxSpeedZ          = 61,
    MaxSpeedE          = 62,
    MaxAccX            = 63,
    MaxAccY            = 64,
    MaxAccZ            = 65,
    MaxAccE            = 66,
    MaxSpeedJerkX      = 67,
    MaxSpeedJerkY      = 68,
    MaxSpeedJerkZ      = 69,
    MaxSpeedJerkE      = 70,
    StepX              = 71,
    StepY              = 72,
    StepZ              = 73,
    StepE              = 74,
    Setspeed           = 75,
    SetZOffset         = 76,
    Rectangle          = 77,
    BLTouch            = 78,
    TempTooLow         = 79,
    AutoLeveling       = 80,
    TempTooHigh        = 81,
    NoTips_C           = 82,
    NoTips_E           = 83,
    Continue_C         = 84,
    Continue_E         = 85,
    Cancel_C           = 86,
    Cancel_E           = 87,
    Confirm_C          = 88,
    Confirm_E          = 89,
    Info_0             = 90,
    Info_1             = 91,

    // Custom icons
    #if ENABLED(DWIN_CREALITY_LCD_CUSTOM_ICONS)
      // Index of every custom icon should be >= CUSTOM_ICON_START
      Checkbox_F       = 200,
      Checkbox_T       = 201,
      Fade             = 202,
      Mesh             = 203,
      Tilt             = 204,
      Brightness       = 205,
      AxisD            = 249,
      AxisBR           = 250,
      AxisTR           = 251,
      AxisBL           = 252,
      AxisTL           = 253,
      AxisC            = 254,
    #else
      Fade             = Version,
      Mesh             = Version,
      Tilt             = Version,
      Brightness       = Version,
      AxisD            = Axis,
      AxisBR           = Axis,
      AxisTR           = Axis,
      AxisBL           = Axis,
      AxisTL           = Axis,
      AxisC            = Axis,
    #endif
  };
  constexpr Icon NoIcon = Icon(0); // note collision with "ICON"
  constexpr Icon CUSTOM_ICON_START = Icon(200);

  enum class Font: uint8_t {
    font6x12  = 0x00,
    font8x16  = 0x01,
    font10x20 = 0x02,
    font12x24 = 0x03,
    font14x28 = 0x04,
    font16x32 = 0x05,
    font20x40 = 0x06,
    font24x48 = 0x07,
    font28x56 = 0x08,
    font32x64 = 0x09,
  };

} // namespace DWIN


// TODO refactor this

#define Custom_Colors       10
#define Color_White         0xFFFF
#define Color_Light_White   0xBDD7
#define Color_Green         0x07E0
#define Color_Light_Green   0x3460
#define Color_Cyan          0x07FF
#define Color_Light_Cyan    0x04F3
#define Color_Blue          0x015F
#define Color_Light_Blue    0x3A6A
#define Color_Magenta       0xF81F
#define Color_Light_Magenta 0x9813
#define Color_Red           0xF800
#define Color_Light_Red     0x8800
#define Color_Orange        0xFA20
#define Color_Light_Orange  0xFBC0
#define Color_Yellow        0xFF0F
#define Color_Light_Yellow  0x8BE0
#define Color_Brown         0xCC27
#define Color_Light_Brown   0x6204
#define Color_Black         0x0000
#define Color_Grey          0x18E3
#define Color_Bg_Window     0x31E8  // Popup background color
#define Color_Bg_Blue       0x1125  // Dark blue background color
#define Color_Bg_Black      0x0841  // Black background color
#define Color_Bg_Red        0xF00F  // Red background color
#define Popup_Text_Color    0xD6BA  // Popup font background color
#define Line_Color          0x3A6A  // Split line color
#define Rectangle_Color     0xEE2F  // Blue square cursor color
#define Percent_Color       0xFE29  // Percentage color
#define BarFill_Color       0x10E4  // Fill color of progress bar
#define Select_Color        0x33BB  // Selected color
#define Check_Color         0x4E5C  // Check-box check color
#define Confirm_Color       0x34B9
#define Cancel_Color        0x3186


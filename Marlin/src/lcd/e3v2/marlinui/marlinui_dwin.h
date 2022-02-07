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

/**
 * lcd/e3v2/marlinui/lcdprint_dwin.h
 */

#include "../../../inc/MarlinConfigPre.h"
#include "dwin_lcd.h"

typedef uint16_t dwin_coord_t;  // Screen can be pretty big
typedef uint16_t lcd_uint_t;
typedef int16_t lcd_int_t;

typedef struct {
  Font index;
  uint8_t width, height;
  uint16_t fg, bg;
  bool solid;
} dwin_font_t;

extern dwin_font_t dwin_font;

// Only Western languages support big / small fonts
//#if DISABLED(DISPLAY_CHARSET_ISO10646_1)
//  #undef USE_BIG_EDIT_FONT
//  #undef USE_SMALL_INFOFONT
//#endif

static const constexpr Font DWIN_FONT_EDIT =
#if ENABLED(USE_BIG_EDIT_FONT)
  Font::f10x20;
#else
  Font::f8x16;
#endif

static const constexpr Font DWIN_FONT_INFO = Font::f8x16;

#define MENU_FONT_WIDTH   (DWIN_FONT_MENU.width())
#define MENU_FONT_HEIGHT  (DWIN_FONT_MENU.height())
#define MENU_FONT_ASCENT  (DWIN_FONT_MENU.ascent())
#define MENU_FONT_DESCENT (DWIN_FONT_MENU.descent())

#define EXTRA_ROW_HEIGHT 8
#define MENU_LINE_HEIGHT (MENU_FONT_HEIGHT + EXTRA_ROW_HEIGHT)

#define EDIT_FONT_WIDTH   (DWIN_FONT_EDIT.width())
#define EDIT_FONT_HEIGHT  (DWIN_FONT_EDIT.height())
#define EDIT_FONT_ASCENT  (DWIN_FONT_EDIT.ascent())
#define EDIT_FONT_DESCENT (DWIN_FONT_EDIT.descent())

#define INFO_FONT_WIDTH   (DWIN_FONT_INFO.width())
#define INFO_FONT_HEIGHT  (DWIN_FONT_INFO.height())
#define INFO_FONT_ASCENT  (DWIN_FONT_INFO.ascent())
#define INFO_FONT_DESCENT (DWIN_FONT_INFO.descent())

#define STAT_FONT_WIDTH   (DWIN_FONT_STAT.width())
#define STAT_FONT_HEIGHT  (DWIN_FONT_STAT.height())
#define STAT_FONT_ASCENT  (DWIN_FONT_STAT.ascent())
#define STAT_FONT_DESCENT (DWIN_FONT_STAT.descent())

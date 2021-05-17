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
#include "dwin_defs.h"
#include "geometry.h"
#include <cstdint>
#include <variant>

using std::variant;

/* Menu handler and menu definitions */

namespace Creality {

  struct MenuTypeList {
  };

  struct MenuTypeIcons {
    const Dimensions size;
    const Grid grid;
    const uint8_t columns;
  };

  struct MenuItem {
    const DWIN::Icon icon;
    const DWIN::Icon selectedIcon;
    const char * const text;
  };

  constexpr MenuItem EndMenu = { DWIN::NoIcon, DWIN::NoIcon, nullptr };

  struct Menu {
    const char * const title;
    const variant<MenuTypeList, MenuTypeIcons> type;
    const MenuItem items[];
  };

  class MenuEngine {
  public:
    void Draw_IconicMenu(const MenuTypeIcons& type, const MenuItem items[], uint16_t selection);
  private:
    void Draw_IconicItem(const MenuTypeIcons& type, const MenuItem& item, Point pos, bool selected);
  };

} // namespace Creality

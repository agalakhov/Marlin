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

  // Menu type tag struct: plain list
  struct MenuTypeList { };

  // Menu type tag struct: icons
  struct MenuTypeIcons {
    const Dimensions size;
    const Grid grid;
    const uint8_t columns;
  };

  // One or two icons for a menu item
  struct MenuIcons {
    DWIN::Icon normal;
    DWIN::Icon selected;
  public:
    constexpr MenuIcons(DWIN::Icon icon)
      : normal(icon)
      , selected(DWIN::NoIcon)
    { }
    constexpr MenuIcons(DWIN::Icon normalIcon, DWIN::Icon selectedIcon)
      : normal(normalIcon)
      , selected(selectedIcon)
    { }
  };

  // Predicate function wrapper. If false, menu item is not displayed.
  struct MenuItemPredicate {
    bool (* const condition)();
    bool invert;
  public:
    constexpr MenuItemPredicate()
      : condition(nullptr)
      , invert(false)
    { }
    constexpr MenuItemPredicate(bool (* const cond)(), bool inv)
      : condition(cond)
      , invert(inv)
    { }
    bool operator()() const {
      return condition ? (invert ? (! condition()) : condition()) : true;
    }
  };

  // Predicate: display menu item only if function returns true
  inline constexpr MenuItemPredicate Only_If(bool (* const cond)()) {
    return MenuItemPredicate(cond, false);
  }

  // Predicate: display menu item only if function returns false
  inline constexpr MenuItemPredicate Only_If_Not(bool (* const cond)()) {
    return MenuItemPredicate(cond, true);
  }

  // Just a item of struct Menu
  struct MenuItem {
    const MenuIcons icons;
    const char * const text;
    const MenuItemPredicate predicate;
  };

  // Sentinel. Each menu MUST end with this one.
  constexpr MenuItem EndMenu = { DWIN::NoIcon, nullptr };

  // Populate this struct as static constant in order to get a menu.
  struct Menu {
    const char * const title;
    const variant<MenuTypeList, MenuTypeIcons> type;
    const MenuItem items[];
  };

  // The menu engine class. This handles all the menus.
  class MenuEngine {
  public:
    void Draw_IconicMenu(const MenuTypeIcons& type, const MenuItem items[], uint16_t selection);
  private:
    void Draw_IconicItem(const MenuTypeIcons& type, const MenuItem& item, Point pos, bool selected);
  };

} // namespace Creality

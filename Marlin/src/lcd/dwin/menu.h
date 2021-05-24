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
  struct MenuType_List { };

  // Menu type tag struct: icons
  struct MenuType_Icons {
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

  struct Menu;

  // Menu item action: Enter submenu
  struct Action_EnterMenu {
    const Menu& menu;
  };

  // Menu item action: go to parent menu
  struct Action_LeaveMenu {
  };

  // Menu item action: execute a function
  struct Action_Do {
    void (* const function)();
  };

  // Menu item action: edit a variable
  struct Action_Edit {
    // TODO
  };

  // Menu item action: function does not exist
  struct Action_Dummy {
  };

  typedef variant<
    Action_EnterMenu,
    Action_LeaveMenu,
    Action_Do,
    Action_Edit,
    Action_Dummy
  > MenuAction;

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
    const MenuAction action;
    const MenuItemPredicate predicate;
  };

  // Sentinel. Each menu MUST end with this one.
  constexpr MenuItem EndMenu = { DWIN::NoIcon, nullptr, Action_Dummy{} };

  // Populate this struct as static constant in order to get a menu.
  struct Menu {
    const char * const title;
    const variant<MenuType_List, MenuType_Icons> type;
    const MenuItem items[];
  };

  // The menu engine class. This handles all the menus.
  class MenuEngine {
  public:
    MenuEngine();
    void EnterMenu(const Menu * menu);
    void LeaveMenu();
    void Redraw();
    void Control();
  public: // TODO private
    static void Draw_IconicMenu(const MenuType_Icons& type, const MenuItem items[], uint16_t selection);
    static void Draw_ListMenu(const MenuType_List& type, const MenuItem items[], uint16_t selection);
  private:
    static void Draw_IconicItem(const MenuType_Icons& type, const MenuItem& item, Point pos, bool selected);
    static void Draw_ListItem(const MenuType_List& type, const MenuItem& item, Point pos, bool selected);
  private:
    MenuEngine(const MenuEngine&) = delete;
    MenuEngine& operator= (const MenuEngine&) = delete;
  private:
    struct StackRec {
      const Menu * menu;
      uint8_t selection; // relative to the scroll position
      uint16_t scroll;
    };
    static constexpr uint8_t MAX_MENU_DEPTH = 8;
    uint8_t stackPos;
    StackRec menuStack[MAX_MENU_DEPTH];
  };

} // namespace Creality

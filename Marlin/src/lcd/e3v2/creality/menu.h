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
#include "dwin_defs.h"
#include "geometry.h"
#include <cstdint>
#include <variant>

using ::std::variant;

/* Menu handler and menu definitions */

namespace Creality {

  // Safe wrapper around a function pointer
  template <typename T>
  class Func {
  public:
    constexpr Func()
      : fp(nullptr)
    { }
    constexpr Func(T (* func)())
      : fp(func)
    { }
    operator bool () const {
      return !!this->fp;
    }
    T operator() () const {
      return (this->fp) ? this->fp() : T();
    }
  private:
    T (* const fp)();
  };

  // Menu type tag struct: plain list
  struct MenuType_List {
    const Func<void> on_enter;
    const Func<void> on_leave;
  };

  // Options for iconic menu
  enum class MenuOptions : uint8_t {
    Simple = 0,
    WithLogo,
  };

  // Menu type tag struct: icons
  struct MenuType_Icons {
    const Dimensions size;
    const Grid grid;
    const uint8_t columns;
    const MenuOptions options;
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
    const Func<bool> condition;
    const bool invert;
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

  // Editable item
  class EditableItem {
    public:
      virtual void Draw(Point position, bool selected) const = 0;
      virtual bool Enter() const { return true; }
      virtual void Step(bool sign) const = 0;
      virtual void Done() const { }
    protected:
      constexpr EditableItem() { }
    private:
      EditableItem(const EditableItem&) = delete;
      EditableItem& operator= (const EditableItem&) = delete;
  };

  // Just editable number
  template <typename T>
  struct EditableNumber : public EditableItem {
    T * const value;
    const T min;
    const T max;
    const T step;
  public:
    constexpr EditableNumber(T * _value, T _min, T _max, T _step)
      : value(_value), min(_min), max(_max), step(_step)
    { }
    virtual void Draw(Point position, bool selected) const;
    virtual void Step(bool sign) const;
  };

  // Editable boolean value
  struct EditableBool : public EditableItem {
    bool * const value;
  public:
    constexpr EditableBool(bool * _value)
      : value(_value)
    { }
    virtual void Draw(Point position, bool selected) const;
    virtual bool Enter() const;
    virtual void Step(bool) const { /* unreachable */ }
  };

  // Menu item action: Enter submenu
  struct Action_EnterMenu {
    const Menu& menu;
  };

  // Menu item action: go to parent menu
  struct Action_LeaveMenu {
  };

  // Menu item action: execute a function
  struct Action_Do {
    const Func<void> function;
  };

  // Menu item action: edit a variable
  struct Action_Value {
    const EditableItem * const editable;
  };

  // Menu item action: function does not exist
  struct Action_Dummy {
  };

  typedef variant<
    const Action_EnterMenu,
    const Action_LeaveMenu,
    const Action_Do,
    const Action_Value,
    const Action_Dummy
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
  private:
    MenuItem(const MenuItem&) = delete;
    MenuItem& operator= (const MenuItem&) = delete;
  };

  // Sentinel. Each menu MUST end with this one.
  //
  // This is a function and not a constant to avoid calling copy constructor.
  inline constexpr MenuItem EndMenu() {
    return { DWIN::NoIcon, nullptr, Action_Dummy{} };
  }

  // Convenience helper for adding "Back" menu items
  inline constexpr MenuItem MenuBack() {
    return { DWIN::Icon::Back, "Back", Action_LeaveMenu{} };
  }

  // Populate this struct as static constant in order to get a menu.
  struct Menu {
    const char * const title;
    const variant<const MenuType_List, const MenuType_Icons> type;
    const MenuItem items[];
  public:
    bool isSentinel(uint16_t index) const { return this->items[index].text == nullptr; }
    bool isLast(uint16_t index) const { return this->isSentinel(index + 1); }
  };

  // The menu engine class. This handles all the menus.
  class MenuEngine {
    struct StackRec {
      const Menu * menu;
      uint16_t scroll;
      uint8_t selection; // relative to the scroll position
    public:
      uint16_t current() const { return this->selection + this->scroll; }
      const MenuItem * firstVisible() const { return this->menu->items + this->scroll; }
    };
  public:
    MenuEngine();
    void EnterMenu(const Menu& menu);
    void LeaveMenu();
    void Redraw();
    void Control();
  private:
    void Control_Edit(const EditableItem& edit);
    void Control_Navigate(StackRec& rec);
    void Perform_Action(const MenuAction& action);
  public: // TODO private
    static void Draw_Title(const char * title);
    // returns number of items drawn
    static uint8_t Draw_Menu(const MenuType_Icons& type, const MenuItem items[], uint16_t selection);
    static uint8_t Draw_Menu(const MenuType_List& type, const MenuItem items[], uint16_t selection);
  private:
    void Redraw_Cursor(uint16_t oldSelection);
    static void Draw_IconicItem(const MenuType_Icons& type, const MenuItem& item, Point pos, bool selected);
    static void Draw_ListItem(const MenuType_List& type, const MenuItem& item, Point pos, bool selected);
    static void Draw_ListCursor(uint16_t selection, bool visible);
  private:
    MenuEngine(const MenuEngine&) = delete;
    MenuEngine& operator= (const MenuEngine&) = delete;
  private:
    static constexpr uint8_t MAX_MENU_DEPTH = 8;
    uint8_t maxItemsOnScreen; // updated on each redraw
    uint8_t stackPos;
    bool isEditing;
    StackRec menuStack[MAX_MENU_DEPTH];
  };

} // namespace Creality

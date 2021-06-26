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

/**
 * DWIN by Creality3D
 * Rewrite and Extui Port by Jacob Myers
 * Refactor by Alexey Galakhov
 */

#include "menu.h"
#include "dwin.h"
#include "rotary_encoder.h"

#include <cmath>

using std::holds_alternative;

namespace Creality {

  template <>
  void EditableNumber<float>::Draw(Point pos, bool selected) const {
    const auto font = Geometry::menuFont;
    const auto fColor = Color_White;
    const auto bColor = (selected) ? Select_Color : Color_Bg_Black;
    if (isnan(*this->value)) {
      DWIN_Draw_String(false, true, font, Color_White, bColor, pos.x, pos.y, " NaN");
    } else {
      const uint8_t fracdigits = -log10(this->step);
      const uint8_t intdigits = 5 - fracdigits;
      const auto w = Geometry::Font_CharSize(font).w;
      const uint16_t val = round(fabs(*this->value) / this->step);
      DWIN_Draw_String(false, true, font, fColor, bColor, pos.x, pos.y, (value < 0) ? "-" : ":");
      DWIN_Draw_FloatValue(true, true, 0, font, fColor, bColor, intdigits, fracdigits, pos.x + w, pos.y, val);
    }
  }

  MenuEngine::MenuEngine()
    : stackPos(0)
  { }

  void MenuEngine::EnterMenu(const Menu& menu) {
    if (this->stackPos < MAX_MENU_DEPTH) {
      {
        const auto* const ty = std::get_if<const MenuType_List>(&menu.type);
        if (ty) {
          ty->on_enter();
        }
      }
      this->menuStack[this->stackPos++] = {
        .menu = &menu,
        .selection = 0,
        .scroll = 0,
      };
    }
    this->Redraw();
    DWIN_UpdateLCD();
  }

  void MenuEngine::LeaveMenu() {
    if (this->stackPos > 0) {
      {
        const auto& rec = this->menuStack[this->stackPos - 1];
        const auto* const ty = std::get_if<const MenuType_List>(&rec.menu->type);
        if (ty) {
          ty->on_leave();
        }
      }
      this->stackPos--;
    }
    this->Redraw();
    DWIN_UpdateLCD();
  }

  void MenuEngine::Redraw() {
    // FIXME
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 31, Geometry::screen.w, 352); // Clear Menu Area

    struct Draw {
      const MenuItem * items;
      uint16_t selection;
    public:
      void operator() (const MenuType_Icons& type) const {
        return MenuEngine::Draw_IconicMenu(type, items, selection);
      }
      void operator() (const MenuType_List& type) const {
        return MenuEngine::Draw_ListMenu(type, items, selection);
      }
    };

    if (this->stackPos > 0) {
      const auto& rec = this->menuStack[this->stackPos - 1];
      Draw_Title(rec.menu->title);
      Draw d {
        rec.menu->items,
        rec.selection,
      };
      std::visit(d, rec.menu->type);
    }
  }

  void MenuEngine::Redraw_Cursor(uint16_t oldSel) {
    struct Draw {
      const MenuItem * items;
      uint16_t oldSelection;
      uint16_t selection;
    public:
      void operator() (const MenuType_Icons& type) {
        MenuEngine::Draw_IconicMenu(type, items, selection);
      }
      void operator() (const MenuType_List&) {
        MenuEngine::Draw_ListCursor(oldSelection, false);
        MenuEngine::Draw_ListCursor(selection, true);
      }
    };

    if (this->stackPos > 0) {
      const auto& rec = this->menuStack[this->stackPos - 1];
      Draw d {
        rec.menu->items,
        oldSel,
        rec.selection,
      };
      std::visit(d, rec.menu->type);
    }
  }

  void MenuEngine::Control() {
    if (this->stackPos == 0) {
      return;
    }
    auto& rec = this->menuStack[this->stackPos - 1];

    switch (Encoder_ReceiveAnalyze()) {
      case ENCODER_DIFF_NO:
        break;
      case ENCODER_DIFF_CCW:
        if (rec.selection > 0) {
          auto oldSel = rec.selection--;
          this->Redraw_Cursor(oldSel);
        } else if (rec.scroll > 0) {
          rec.scroll--;
          this->Redraw();
        }
        DWIN_UpdateLCD();
        break;
      case ENCODER_DIFF_CW:
        if (rec.selection < 5) {
          auto oldSel = rec.selection++;
          this->Redraw_Cursor(oldSel);
        } else if (rec.scroll < 0) {
          rec.scroll++;
          this->Redraw();
        }
        DWIN_UpdateLCD();
        break;
      case ENCODER_DIFF_ENTER:
        const auto& action = rec.menu->items[rec.selection].action;
        this->Perform_Action(action);
        break;
    }
  }

  void MenuEngine::Perform_Action(const MenuAction& action) {
    struct Actor {
      MenuEngine* self;
    public:
      void operator() (const Action_EnterMenu& enter) {
        self->EnterMenu(enter.menu);
      }
      void operator() (const Action_LeaveMenu&) {
        self->LeaveMenu();
      }
      void operator() (const Action_Do& func) {
        func.function();
      }
      void operator() (const Action_Value& edit) {
        // TODO
      }
      void operator() (const Action_Dummy&) {
      }
    };

    std::visit(Actor { this }, action);
  }

  void MenuEngine::Draw_IconicMenu(const MenuType_Icons& type, const MenuItem items[], uint16_t selection) {
    if (type.options == MenuOptions::WithLogo) {
      DWIN_ICON_Show(DWIN::ICON, DWIN::Icon::LOGO, Geometry::logoPos.x, Geometry::logoPos.y);
    }

    uint8_t idx = 0;
    uint8_t column = 0;
    Point pos = type.grid.origin;
    for (const MenuItem * item = items; item->text != nullptr; ++item) {
      if (item->predicate()) {
        Draw_IconicItem(type, *item, pos, (idx == selection));
        ++column;
        if (column < type.columns) {
          pos.x += type.grid.step.w;
        } else {
          column = 0;
          pos.x = type.grid.origin.x;
          pos.y += type.grid.step.h;
        }
        ++idx;
      }
    }
  }

  void MenuEngine::Draw_ListMenu(const MenuType_List& type, const MenuItem items[], uint16_t selection) {
    uint8_t idx = 0;
    Point pos = {
      Geometry::listItemCursorWidth + Geometry::listItemLeftMargin + Geometry::listItemLeftPadding,
      Geometry::titleHeight + Geometry::listItemPadding
    };
    for (const MenuItem * item = items; idx < 4 && item->text != nullptr; ++item) {
      if (item->predicate()) {
        Draw_ListItem(type, *item, pos, (idx == selection));
        pos.y += Geometry::listItemIconSize.h + 2 * Geometry::listItemPadding + 1;
        ++idx;
      }
    }
    Draw_ListCursor(selection, true);
  }

  void MenuEngine::Draw_IconicItem(const MenuType_Icons& type, const MenuItem& item, Point pos, bool selected) {
    constexpr auto font = Geometry::menuFont;
    const auto highlightColor = Color_White; // TODO GetColor(eeprom_settings.highlight_box, Color_White);
    const auto rect = type.size.AtPoint(pos);
    const auto icon = (selected && item.icons.selected != DWIN::NoIcon) ? item.icons.selected : item.icons.normal;
    DWIN_ICON_Show(DWIN::ICON, icon, pos.x, pos.y);
    if (selected) {
      DWIN_Draw_Rectangle(0, highlightColor, rect.tl.x, rect.tl.y, rect.br.x, rect.br.y);
    }
    const Rectangle textRect = rect.Bottom(Geometry::iconMenuTextHeight);
    const auto textPos = Geometry::Text_Inscribe(textRect, font, item.text);
    DWIN_Draw_String(false, false, font, Color_White, Color_Bg_Blue, textPos.x, textPos.y, item.text);
  }

  void MenuEngine::Draw_ListItem(const MenuType_List& type, const MenuItem& item, Point pos, bool selected) {
    constexpr auto font = Geometry::menuFont;
    const auto lineColor = Line_Color; // TODO GetColor(eeprom_settings.menu_split_line, Line_Color, true);
    const auto icon = (selected && item.icons.selected != DWIN::NoIcon) ? item.icons.selected : item.icons.normal;
    const auto iconPos = pos;
    const Point textPos = {
      static_cast<uint16_t>(pos.x + Geometry::listItemIconSize.w + Geometry::listItemTextOffset),
      static_cast<uint16_t>(pos.y + (Geometry::listItemIconSize.h - Geometry::Font_CharSize(font).h) / 2)
    };
    DWIN_Draw_String(false, false, font, Color_White, Color_Bg_Black, textPos.x, textPos.y, item.text);
    if (icon != DWIN::NoIcon) {
      DWIN_ICON_Show(DWIN::ICON, icon, iconPos.x, iconPos.y);
    }
    if (holds_alternative<const Action_EnterMenu>(item.action)) {
      DWIN_ICON_Show(DWIN::ICON, DWIN::Icon::More, iconPos.x + Geometry::listItemRightIconOffset, iconPos.y);
    }
    {
      const auto* const editable = std::get_if<const Action_Value>(&item.action);
      if (editable) {
        const Point editablePos = {
          static_cast<uint16_t>(textPos.x - Geometry::listItemTextOffset + Geometry::listItemValueOffset),
          textPos.y
        };
        editable->editable->Draw(editablePos, false);
      }
    }
    const uint16_t lineY = pos.y + Geometry::listItemIconSize.h + Geometry::listItemPadding;
    DWIN_Draw_Line(lineColor, Geometry::listItemLineLeft, lineY, Geometry::listItemLineRight, lineY);
  }

  void MenuEngine::Draw_ListCursor(uint16_t selection, bool visible) {
    const auto color = visible ? Rectangle_Color : Color_Bg_Black; // GetColor(eeprom_settings.cursor_color)
    constexpr uint16_t h = Geometry::listItemIconSize.h + 2 * Geometry::listItemPadding + 1;
    const uint16_t y = Geometry::titleHeight + selection * h;
    DWIN_Draw_Rectangle(1, color, 0, y + 1, Geometry::listItemCursorWidth, y + h - 1);
  }

  void MenuEngine::Draw_Title(const char * title) {
    const auto p = Geometry::Text_Inscribe({ Geometry::screen.w, Geometry::titleHeight }, Geometry::titleFont, title);
    const auto color = Color_White; // GetColor(eeprom_settings.menu_top_txt, Color_White, false)
    DWIN_Draw_String(false, false, Geometry::titleFont, color, Color_Bg_Blue, p.x, p.y, title);
  }

} // namespace Creality

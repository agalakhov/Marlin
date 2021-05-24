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

using std::holds_alternative;

namespace Creality {

  MenuEngine::MenuEngine()
    : stackPos(0)
  { }

  void MenuEngine::EnterMenu(const Menu * menu) {
    if (this->stackPos < MAX_MENU_DEPTH) {
      this->menuStack[this->stackPos++] = {
        .menu = menu,
        .selection = 0,
        .scroll = 0,
      };
    }
    this->Redraw();
  }

  void MenuEngine::LeaveMenu() {
    if (this->stackPos > 0) {
      this->stackPos--;
    }
    this->Redraw();
  }

  void MenuEngine::Redraw() {
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
      Draw d {
        rec.menu->items,
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
          rec.selection--;
        } else if (rec.scroll > 0) {
          rec.scroll--;
        }
        break;
      case ENCODER_DIFF_CW:
        if (rec.selection < 5) {
          rec.selection++;
        } else if (rec.scroll < 0) {
          rec.scroll++;
        }
        break;
      case ENCODER_DIFF_ENTER:
        break;
    }

    this->Redraw();
  }

  void MenuEngine::Draw_IconicMenu(const MenuType_Icons& type, const MenuItem items[], uint16_t selection) {
    uint8_t idx = 0;
    uint8_t column = 0;
    Point pos = type.grid.origin;
    for (const MenuItem * item = items; item->text != nullptr; ++item) {
      if (item->predicate()) {
        ++idx;
        Draw_IconicItem(type, *item, pos, (idx == selection));
        ++column;
        if (column < type.columns) {
          pos.x += type.grid.step.w;
        } else {
          column = 0;
          pos.x = type.grid.origin.x;
          pos.y += type.grid.step.h;
        }
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
        ++idx;
        Draw_ListItem(type, *item, pos, (idx == selection));
        pos.x += Geometry::listItemIconSize.h + 2 * Geometry::listItemPadding + 1;
      }
    }
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
    if (holds_alternative<Action_EnterMenu>(item.action)) {
      DWIN_ICON_Show(DWIN::ICON, DWIN::Icon::More, iconPos.x + Geometry::listItemRightIconOffset, iconPos.y);
    }
    const uint16_t lineY = pos.y + Geometry::listItemIconSize.h + Geometry::listItemPadding;
    DWIN_Draw_Line(lineColor, Geometry::listItemLineLeft, lineY, Geometry::listItemLineRight, lineY);
  }

} // namespace Creality

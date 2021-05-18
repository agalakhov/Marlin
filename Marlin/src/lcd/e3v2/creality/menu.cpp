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

namespace Creality {

  void MenuEngine::Draw_IconicMenu(const MenuTypeIcons& type, const MenuItem items[], uint16_t selection) {
    uint8_t idx = 0;
    uint8_t column = 0;
    Point pos = type.grid.origin;
    for (const MenuItem * item = items; idx < 4 && item->text != nullptr; ++item) {
      if (item->predicate()) {
        ++idx;
        this->Draw_IconicItem(type, *item, pos, (idx == selection));
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

  void MenuEngine::Draw_IconicItem(const MenuTypeIcons& type, const MenuItem& item, Point pos, bool selected) {
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

} // namespace Creality

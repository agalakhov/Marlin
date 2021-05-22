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

#include "creality_menu.h"

using namespace Creality;
using DWIN::Icon;

const Menu Creality_mainMenu = {
  "Main Menu",
  MenuType_Icons {
      Geometry::mainMenuItemSize,
      Geometry::mainMenuGrid,
      2
  },
  {
    { { Icon::Print_0, Icon::Print_1 }, "Print", Action_Dummy{} },
    { { Icon::Prepare_0, Icon::Prepare_1 }, "Prepare", Action_Dummy{} },
    { { Icon::Control_0, Icon::Control_1 }, "Control", Action_Dummy{} },
    #if ANY(HAS_ONESTEP_LEVELING, AUTO_BED_LEVELING_UBL, PROBE_MANUALLY)
      { { Icon::Leveling_0, Icon::Leveling_1 }, "Level", Action_Dummy{} },
    #else
      { { Icon::Info_0, Icon::Info_1}, "Info", Action_Dummy{} },
    #endif
    Creality::EndMenu
  }
};

extern bool printing;
static bool Helper_IsPrinting() {
  return printing;
}

const Menu Creality_printMenu = {
  nullptr,
  Creality::MenuType_Icons {
      Geometry::printMenuItemSize,
      Geometry::printMenuGrid,
      3
  },
  {
    { { Icon::Setup_0, Icon::Setup_1 }, "Tune", Action_Dummy{} },
    { { Icon::Pause_0, Icon::Pause_1 }, "Pause", Action_Dummy{}, Only_If(Helper_IsPrinting) },
    { { Icon::Continue_0,  Icon::Continue_1}, "Print", Action_Dummy{}, Only_If_Not(Helper_IsPrinting) },
    { { Icon::Stop_0, Icon::Stop_1}, "Stop", Action_Dummy{} },
    Creality::EndMenu
  }
};

static const Menu moveMenu = {
  "Move",
  MenuType_List{},
  {
    { Icon::Back,         "Back",             Action_LeaveMenu{} },
    // TODO
    EndMenu
  }
};

static const Menu homeMenu = {
  "Homing Menu",
  MenuType_List{},
  {
    { Icon::Back,         "Back",             Action_LeaveMenu{} },
    // TODO
    EndMenu
  }
};

static const Menu preheatMenu = {
  "Preheat",
  MenuType_List{},
  {
    { Icon::Back,         "Back",             Action_LeaveMenu{} },
    // TODO
    EndMenu
  }
};

static const Menu prepareMenu = {
  "Prepare",
  MenuType_List{},
  {
    { Icon::Back,         "Back",             Action_LeaveMenu{} },
    { Icon::Axis,         "Move",             Action_EnterMenu{moveMenu} },
//  { Icon::CloseMotor, "Disable Stepper",  Action_GCode{"M84"} },
    { Icon::SetHome,      "Homing",           Action_EnterMenu{homeMenu} },

//  { Icon::PrintSize,  "Manual Leveling",  Action_Do{ manual leveling },
/*         if (axes_should_home()) {
              Popup_Handler(Home);
              gcode.home_all_axes(true);
            }
            #if HAS_LEVELING
              level_state = planner.leveling_active;
              set_bed_leveling_enabled(false);
            #endif
            Draw_Menu(ManualLevel);
          }
*/

    #if HAS_ZOFFSET_ITEM
//  { Icon::Zoffset,      "Z-Offset",         Action_Do{ z_offset } },
/*
              #if HAS_LEVELING
                level_state = planner.leveling_active;
                set_bed_leveling_enabled(false);
              #endif
              Draw_Menu(ZOffset);
*/
    #endif

    #if HAS_PREHEAT
      { Icon::Temperature,    "Preheat",      Action_EnterMenu{preheatMenu} },
//    { Icon::Cool,           "Cooldown",     Action_Do{ cooldown }, 
/*
  thermalManager.zero_fan_speeds();
  thermalManager.disable_all_heaters();
 */
    #endif

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      { Icon::ResumeEEPROM, "Change Filament", 
        #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
          Action_EnterMenu{changeFilamentMenu},
        #else
        /*
                if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp) {
                  Popup_Handler(ETemp);
                }
                else {
                  if (thermalManager.temp_hotend[0].celsius < thermalManager.temp_hotend[0].target-2) {
                    Popup_Handler(Heating);
                    thermalManager.wait_for_hotend(0);
                  }
                  Popup_Handler(FilChange);
                  char buf[20];
                  sprintf(buf, "M600 B1 R%i", thermalManager.temp_hotend[0].target);
                  gcode.process_subcommands_now_P(buf);
                }
         */
        #endif
      },
    #endif

    EndMenu
  }
};

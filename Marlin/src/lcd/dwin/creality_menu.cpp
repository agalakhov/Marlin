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
#include "creality_actions.h"
#include "creality_dwin.h"
#include "../../inc/MarlinConfigPre.h"

using namespace Creality;
using DWIN::Icon;

// Menu structures MUST land in .rodata section. Make sure they all are const constexpr.

static const constexpr Creality::Menu moveMenu = {
  "Move",
  MenuType_List{
    .on_enter = nullptr,
    .on_leave = CrealityActions::RetractProbe,
  },
  {
    MenuBack(),
//TODO    { Icon::MoveX,  "Move X",   Action_Value{current_position.x, X_MIN_POS, X_MAX_POS, 10} },
//TODO    { Icon::MoveY,  "Move Y",   Action_Value{current_position.y, Y_MIN_POS, Y_MAX_POS, 10} },
//TODO    { Icon::MoveZ,  "Move Z",   Action_Value{current_position.z, Z_MIN_POS, Z_MAX_POS, 10} },
    #if HAS_HOTEND
// TODO      { Icon::Extruder, "Extruder", Action_Value
    #endif
    #if HAS_BED_PROBE
//TODO      { Icon::StockConfiguration, "Probe", Action_Checkbox{probe_deployed},
    #endif
//TODO    { Icon::Axis,   "Live Movement",    Action_Checkbox{livemove} },
    EndMenu()
  }
};

static const constexpr Creality::Menu homeMenu = {
  "Homing Menu",
  MenuType_List{},
  {
    MenuBack(),
    { Icon::Homing,     "Home All",           Action_Do{CrealityActions::HomeAll} },
//TODO    { Icon::MoveX,      "Home X",             Action_GCode_Sync{Home, "G28 X"} },
//TODO    { Icon::MoveY,      "Home Y",             Action_GCode_Sync{Home, "G28 Y"} },
//TODO    { Icon::MoveZ,      "Home Z",             Action_GCode_Sync{Home, "G28 Z"} },
//TODO    { Icon::SetHome,    "Set Home Position",  Action_GCode_Beep{"G92 X0 Y0 Z0"} },
    EndMenu()
  }
};

static const constexpr Creality::Menu manualLevelingMenu = {
  "Manual Leveling",
  MenuType_List {
    .on_enter = CrealityActions::HomeAndDisableLeveling,
    .on_leave = CrealityActions::ReEnableLeveling,
  },
  {
    MenuBack(),
    // TODO
    EndMenu()
  }
};

static const constexpr Creality::Menu zOffsetMenu = {
  "Z Offset",
  MenuType_List {
      .on_enter = CrealityActions::DisableLeveling,
      .on_leave = CrealityActions::ReEnableLeveling,
  },
  {
    MenuBack(),
    // TODO
    EndMenu()
  }
};

static const constexpr Creality::Menu preheatMenu = {
  "Preheat",
  MenuType_List{},
  {
    MenuBack(),
    // TODO
    EndMenu()
  }
};

static const constexpr Creality::Menu prepareMenu = {
  "Prepare",
  MenuType_List{},
  {
    MenuBack(),
    { Icon::Axis,       "Move",             Action_EnterMenu{moveMenu}              },
//TODO    { Icon::CloseMotor,   "Disable Stepper",  Action_GCode{"M84"} },
    { Icon::SetHome,    "Homing",           Action_EnterMenu{homeMenu}              },
    { Icon::PrintSize,  "Manual Leveling",  Action_EnterMenu{manualLevelingMenu}    },

    #if HAS_ZOFFSET_ITEM
    { Icon::Zoffset,      "Z-Offset",         Action_EnterMenu{zOffsetMenu}         },
    #endif

    #if HAS_PREHEAT
      { Icon::Temperature,    "Preheat",      Action_EnterMenu{preheatMenu}         },
      { Icon::Cool,           "Cooldown",     Action_Do{CrealityActions::Cooldown}  },
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

    EndMenu()
  }
};

const constexpr Creality::Menu Creality_mainMenu = {
  "Main Menu",
  MenuType_Icons {
      Geometry::mainMenuItemSize,
      Geometry::mainMenuGrid,
      2
  },
  {
    { { Icon::Print_0, Icon::Print_1 },         "Print",    Action_Do{ CrealityDWINClass::Tmp_PrintClicked }    },
    { { Icon::Prepare_0, Icon::Prepare_1 },     "Prepare",  Action_Do{ CrealityDWINClass::Tmp_PrepareClicked }  },
    { { Icon::Control_0, Icon::Control_1 },     "Control",  Action_Do{ CrealityDWINClass::Tmp_SettingsClicked } },
    #if ANY(HAS_ONESTEP_LEVELING, AUTO_BED_LEVELING_UBL, PROBE_MANUALLY)
      { { Icon::Leveling_0, Icon::Leveling_1 }, "Level",    Action_Do{ CrealityDWINClass::Tmp_LevelClicked }    },
    #else
      { { Icon::Info_0, Icon::Info_1},          "Info",     Action_Do{ CrealityDWINClass::Tmp_InfoClicked }     },
    #endif
    EndMenu()
  }
};

extern bool printing;
static bool Helper_IsPrinting() {
  return printing;
}

const constexpr Creality::Menu Creality_printMenu = {
  nullptr,
  Creality::MenuType_Icons {
      Geometry::printMenuItemSize,
      Geometry::printMenuGrid,
      3
  },
  {
    { { Icon::Setup_0, Icon::Setup_1 },         "Tune",     Action_Dummy{} },
    { { Icon::Pause_0, Icon::Pause_1 },         "Pause",    Action_Dummy{}, Only_If(Helper_IsPrinting) },
    { { Icon::Continue_0,  Icon::Continue_1},   "Print",    Action_Dummy{}, Only_If_Not(Helper_IsPrinting) },
    { { Icon::Stop_0, Icon::Stop_1},            "Stop",     Action_Dummy{} },
    EndMenu()
  }
};

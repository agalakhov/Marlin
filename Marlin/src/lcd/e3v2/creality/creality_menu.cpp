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
#include "../../../inc/MarlinConfigPre.h"

#include "../../../module/planner.h"
#include "../../../module/temperature.h"
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

using namespace Creality;
using DWIN::Icon;

// Menu structures MUST land in .rodata section. Make sure they all are const constexpr.

static const constexpr Creality::EditableNumber<float> curPosX { &current_position.x, X_MIN_POS, X_MAX_POS, 0.1 };
static const constexpr Creality::EditableNumber<float> curPosY { &current_position.y, X_MIN_POS, X_MAX_POS, 0.1 };
static const constexpr Creality::EditableNumber<float> curPosZ { &current_position.z, X_MIN_POS, X_MAX_POS, 0.1 };

static const constexpr Creality::Menu moveMenu {
  "Move",
  MenuType_List{
    .on_enter = nullptr,
    .on_leave = CrealityActions::RetractProbe,
  },
  {
    MenuBack(),
    { Icon::MoveX,  "Move X",   Action_Value{&curPosX} },
    { Icon::MoveY,  "Move Y",   Action_Value{&curPosY} },
    { Icon::MoveZ,  "Move Z",   Action_Value{&curPosZ} },
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

static const constexpr Creality::Menu homeMenu {
  "Homing Menu",
  MenuType_List{},
  {
    MenuBack(),
    { Icon::Homing,     "Home All",           Action_Do{CrealityActions::HomeAll} },
    { Icon::MoveX,      "Home X",             Action_GCode_Sync{Home, "G28 X"}    },
    { Icon::MoveY,      "Home Y",             Action_GCode_Sync{Home, "G28 Y"}    },
    { Icon::MoveZ,      "Home Z",             Action_GCode_Sync{Home, "G28 Z"}    },
//TODO    { Icon::SetHome,    "Set Home Position",  Action_GCode_Beep{"G92 X0 Y0 Z0"} },
    EndMenu()
  }
};

static const constexpr Creality::Menu manualLevelingMenu {
  "Manual Leveling",
  MenuType_List {
    .on_enter = CrealityActions::HomeAndDisableLeveling,
    .on_leave = LevelingDisabler::ReEnable,
  },
  {
    MenuBack(),
    /*{ Icon::Zoffset, "Use Probe", Action_Value{&useProbe} },
              use_probe = !use_probe;
              Draw_Checkbox(row, use_probe);
              if (use_probe) {
                Popup_Handler(Level);
                corner_avg = 0;
                corner_avg += probe.probe_at_point(32.5f, 32.5f, PROBE_PT_RAISE);
                corner_avg += probe.probe_at_point(32.5f, (Y_BED_SIZE + Y_MIN_POS) - 32.5f, PROBE_PT_RAISE);
                corner_avg += probe.probe_at_point((X_BED_SIZE + X_MIN_POS) - 32.5f, (Y_BED_SIZE + Y_MIN_POS) - 32.5f, PROBE_PT_RAISE);
                corner_avg += probe.probe_at_point((X_BED_SIZE + X_MIN_POS) - 32.5f, 32.5f, PROBE_PT_STOW);
                corner_avg /= 4;
                Redraw_Menu();
              }
    */
    { Icon::AxisBL,     "Bottom Left",  Action_Do{ManualLevel::ProbeBL} },
    { Icon::AxisTL,     "Top Left",     Action_Do{ManualLevel::ProbeTL} },
    { Icon::AxisTR,     "Top Right",    Action_Do{ManualLevel::ProbeTR} },
    { Icon::AxisBR,     "Bottom Right", Action_Do{ManualLevel::ProbeBR} },
    { Icon::AxisC,      "Center",       Action_Do{ManualLevel::ProbeC}  },
    //{ Icon::SetZOffset, "Z Position",   Action_Value{&ManualLevel::zPositionEdit} },
    EndMenu()
  }
};

static const constexpr Creality::Menu zOffsetMenu {
  "Z Offset",
  MenuType_List {
      .on_enter = LevelingDisabler::Disable,
      .on_leave = LevelingDisabler::ReEnable,
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

static const constexpr Creality::Menu changeFilamentMenu {
    "Change Filament",
    MenuType_List{},
    {
        MenuBack(),
        { Icon::WriteEEPROM,    "Load Filament",    Action_Do{FilamentLoader::Load} },
        { Icon::ReadEEPROM,     "Unload Filament",  Action_Do{FilamentLoader::Unload} },
        { Icon::ResumeEEPROM,   "Change Filament",  Action_Do{FilamentLoader::Change} },
        EndMenu()
    }
};

static const constexpr Creality::Menu prepareMenu {
  "Prepare",
  MenuType_List{},
  {
    MenuBack(),
    { Icon::Axis,       "Move",             Action_EnterMenu{moveMenu}           },
    { Icon::CloseMotor, "Disable Stepper",  Action_GCode_Queue{"M84"}            },
    { Icon::SetHome,    "Homing",           Action_EnterMenu{homeMenu}           },
    { Icon::PrintSize,  "Manual Leveling",  Action_EnterMenu{manualLevelingMenu} },

    #if HAS_ZOFFSET_ITEM
      { Icon::Zoffset,  "Z-Offset",         Action_EnterMenu{zOffsetMenu}        },
    #endif

    #if HAS_PREHEAT
      { Icon::Temperature,  "Preheat",      Action_EnterMenu{preheatMenu}        },
      { Icon::Cool,         "Cooldown",     Action_Do{CrealityActions::Cooldown} },
    #endif

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      { Icon::ResumeEEPROM, "Change Filament", 
        #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
          Action_EnterMenu{changeFilamentMenu},
        #else
          Action_Do{FilamentLoader::Change}
        #endif
      },
    #endif

    EndMenu()
  }
};

#if HAS_HOTEND
  static const constexpr Creality::Menu hotendPIDMenu = {
    "Hotend PID Settings",
    MenuType_List{},
    {
      MenuBack(),
      // TODO
      EndMenu()
    }
  };
#endif

#if HAS_HEATED_BED
  static const constexpr Creality::Menu bedPIDMenu = {
    "Bed PID settings",
    MenuType_List{},
    {
      MenuBack(),
      // TODO
      EndMenu()
    }
  };
#endif

#if ANY(HAS_HOTEND, HAS_HEATED_BED)
  static uint8_t pid_autotune_cycles = 5;

  static const constexpr Creality::EditableNumber<uint8_t> autotuneCycles { &pid_autotune_cycles, 3, 50, 1 };

  static const constexpr Creality::Menu pidMenu {
    "PID",
    MenuType_List{},
    {
      MenuBack(),

      #if HAS_HOTEND
      { Icon::HotendTemp,   "Hotend",           Action_EnterMenu{hotendPIDMenu} },
      #endif

      #if HAS_HEATED_BED
      { Icon::BedTemp,      "Bed",              Action_EnterMenu{bedPIDMenu}    },
      #endif

      { Icon::FanSpeed,     "Autotune Cycles",  Action_Value{&autotuneCycles} },

      EndMenu()
    }
  };
#endif

#if HAS_HOTEND
  static const constexpr celsius_t MIN_E_TEMP = 0;
  static const constexpr celsius_t MAX_E_TEMP = (HEATER_0_MAXTEMP - HOTEND_OVERSHOOT);
  static const constexpr Creality::EditableNumber<celsius_t> hotendTemp { &thermalManager.temp_hotend[0].target, MIN_E_TEMP, MAX_E_TEMP, 1 };
#endif

#if HAS_HEATED_BED
  static const constexpr celsius_t MIN_BED_TEMP = 0;
  static const constexpr celsius_t MAX_BED_TEMP = BED_MAXTEMP;
  static const constexpr Creality::EditableNumber<celsius_t> bedTemp { &thermalManager.temp_bed.target, MIN_BED_TEMP, MAX_BED_TEMP, 1 };
#endif

#if HAS_FAN
  static const constexpr uint8_t MIN_FAN_SPEED = 0;
  static const constexpr uint8_t MAX_FAN_SPEED = 255;
  static const constexpr Creality::EditableNumber<uint8_t> fanSpeed { &thermalManager.fan_speed[0], MIN_FAN_SPEED, MAX_FAN_SPEED, 1 };
#endif

static const constexpr Creality::Menu temperatureMenu {
  "Temperature",
  MenuType_List{},
  {
    MenuBack(),

    #if HAS_HOTEND
      { Icon::SetEndTemp,   "Hotend",   Action_Value{&hotendTemp}    },
    #endif

    #if HAS_HEATED_BED
      { Icon::SetBedTemp,   "Bed",      Action_Value{&bedTemp}       },
    #endif

    #if HAS_FAN
      { Icon::FanSpeed,     "Fan",      Action_Value{&fanSpeed}       },
    #endif

    #if ANY(HAS_HOTEND, HAS_HEATED_BED)
      { Icon::Step,         "PID",      Action_EnterMenu{pidMenu}   },
    #endif

    // TODO

    EndMenu()
  }
};

static const constexpr Creality::Menu motionMenu = {
  "Motion",
  MenuType_List{},
  {
    MenuBack(),
    // TODO
    EndMenu()
  }
};

static const constexpr Creality::Menu visualMenu = {
  "Visual",
  MenuType_List{},
  {
    MenuBack(),
    // TODO
    EndMenu()
  }
};

static const constexpr Creality::Menu probeMenu = {
  "Probe",
  MenuType_List{},
  {
    MenuBack(),
    // TODO
    EndMenu()
  }
};

static const constexpr Creality::EditableBoolGetSet lcdBeeper {
  []() { return CrealityDWIN.eeprom_settings.beeperenable; },
  [](bool x) { CrealityDWIN.eeprom_settings.beeperenable = x; },
};
extern float corner_pos; // FIXME
static const constexpr Creality::EditableNumber<float> cornerPos { &corner_pos, 1, 100, 0.1 };
#if ENABLED(LIN_ADVANCE)
  static const constexpr Creality::EditableNumber<float> extruderAdvanceK { &planner.extruder_advance_K[0], 0, 10, 0.01 };
#endif
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  static const constexpr Creality::EditableNumber<float> loadLength { &fc_settings[0].load_length, 0, EXTRUDE_MAXLENGTH, 1 };
  static const constexpr Creality::EditableNumber<float> unloadLength { &fc_settings[0].unload_length, 0, EXTRUDE_MAXLENGTH, 1 };
#endif
#if ENABLED(PREVENT_COLD_EXTRUSION)
  static const constexpr Creality::EditableNumber<celsius_t> minExtrusionT { &thermalManager.extrude_min_temp, 0, MAX_E_TEMP, 1 }; 
#endif
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  static const constexpr Creality::EditableBool runoutEnabled { &runout.enabled };
  #if ENABLED(HAS_FILAMENT_RUNOUT_DISTANCE)
    static const constexpr Creality::EditableNumber<float> runoutDistance { &runout.runout_distance(), 0, 999, 0.1 };
  #endif
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  static const constexpr Creality::EditableBool recoveryEnabled { &recovery.enabled };
#endif

static const constexpr Creality::Menu advancedMenu = {
  "Advanced",
  MenuType_List{},
  {
    MenuBack(),
    { Icon::Version,  "LCD Beeper",   Action_Value{&lcdBeeper} },
    #if HAS_BED_PROBE
      { Icon::StepX,    "Probe",    Action_EnterMenu{&probeMenu} },
    #endif
    { Icon::MaxAccelerated, "Bed Screw Inset",  Action_Value{&cornerPos} },
    #if ENABLED(LIN_ADVANCE)
      { Icon::MaxAccelerated,   "Lin Advance Kp",   Action_Value{&extruderAdvanceK} },
    #endif
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      { Icon::WriteEEPROM,  "Load Length",      Action_Value{&loadLength} },
      { Icon::ReadEEPROM,   "Unload Length",    Action_Value{&unloadLength} },
    #endif
    #if ENABLED(PREVENT_COLD_EXTRUSION)
      { Icon::Cool,     "Min Extrusion T",      Action_Value{&minExtrusionT} },
    #endif
    #if ENABLED(FILAMENT_RUNOUT_SENSOR)
      { Icon::Extruder, "Filament Sensor",      Action_Value{&runoutEnabled} },
      #if ENABLED(HAS_FILAMENT_RUNOUT_DISTANCE)
        { Icon::MaxAccE,    "Runout Distance",  Action_Value{&runoutDistance} },
      #endif
    #endif
    #if ENABLED(POWER_LOSS_RECOVERY)
      { Icon::Motion,   "Power-loss Recovery",  Action_Value{&recoveryEnabled} },
    #endif
    EndMenu()
  }
};

static const constexpr Creality::Menu controlMenu {
  "Control",
  MenuType_List{},
  {
    MenuBack(),
    { Icon::Temperature,    "Temperature",  Action_EnterMenu{temperatureMenu}   },
    { Icon::Motion,         "Motion",       Action_EnterMenu{motionMenu}        },
    { Icon::PrintSize,      "Visual",       Action_EnterMenu{visualMenu}        },
    { Icon::Version,        "Advanced",     Action_EnterMenu{advancedMenu}      },
    #if ENABLED(EEPROM_SETTINGS)
      { Icon::WriteEEPROM,  "Store Settings",   Action_Do{CrealityActions::SaveEEPROM}          },
      { Icon::ReadEEPROM,   "Restore Settings", Action_Do{CrealityActions::ReadEEPROM}          },
      { Icon::Temperature,  "Reset to Defaults",    Action_Do{CrealityActions::ResetSettings}   },
    #endif
    EndMenu()
  }
};

const constexpr Creality::Menu Creality_mainMenu {
  "Main Menu",
  MenuType_Icons {
      Geometry::mainMenuItemSize,
      Geometry::mainMenuGrid,
      2,
      MenuOptions::WithLogo
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

const constexpr Creality::Menu Creality_printMenu {
  nullptr,
  Creality::MenuType_Icons {
      Geometry::printMenuItemSize,
      Geometry::printMenuGrid,
      3,
      MenuOptions::Simple
  },
  {
    { { Icon::Setup_0, Icon::Setup_1 },         "Tune",     Action_Dummy{} },
    { { Icon::Pause_0, Icon::Pause_1 },         "Pause",    Action_Dummy{}, Only_If(Helper_IsPrinting) },
    { { Icon::Continue_0,  Icon::Continue_1},   "Print",    Action_Dummy{}, Only_If_Not(Helper_IsPrinting) },
    { { Icon::Stop_0, Icon::Stop_1},            "Stop",     Action_Dummy{} },
    EndMenu()
  }
};

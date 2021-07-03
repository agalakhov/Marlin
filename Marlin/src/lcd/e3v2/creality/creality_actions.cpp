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

#include "creality_actions.h"
#include "popup.h"

#include "../../../inc/MarlinConfigPre.h"

#include "../../../MarlinCore.h"
#include "../../../gcode/gcode.h"
#include "../../../module/planner.h"
#include "../../../module/temperature.h"

#include <cstdarg>

void Execute_GCode(const char * fmt, ...) {
  static char buf[64];
  va_list vl;
  va_start(vl, fmt);
  vsnprintf(buf, sizeof(buf), fmt, vl);
  va_end(vl);
  gcode.process_subcommands_now(buf);
  planner.synchronize();
}

void CrealityActions::Cooldown() {
  thermalManager.zero_fan_speeds();
  thermalManager.disable_all_heaters();
}

void CrealityActions::HomeAll() {
  gcode.home_all_axes(true);
}

void CrealityActions::RetractProbe() {
    // TODO
}

void CrealityActions::HomeIfNeeded() {
    // TODO
}

namespace Creality {
  void LevelingDisabler::Disable() {
    #if HAS_LEVELING
      levelingWasEnabled = planner.leveling_active;
      set_bed_leveling_enabled(false);
    #endif
  }

  void LevelingDisabler::ReEnable() {
    #if HAS_LEVELING
      set_bed_leveling_enabled(levelingWasEnabled);
    #endif
  }

  void FilamentLoader::Load() {
    if (CheckTemperature()) {
      POPUP_WAITING(popupEngine, FilLoad) {
        Execute_GCode("M701");
      }
    }
  }

  void FilamentLoader::Unload() {
    if (CheckTemperature()) {
      POPUP_WAITING(popupEngine, FilLoad, true) {
        Execute_GCode("M702");
      }
    }
  }

  void FilamentLoader::Change() {
    if (CheckTemperature()) {
      POPUP_WAITING(popupEngine, FilChange) {
        Execute_GCode("M600 B1 R%i", thermalManager.temp_hotend[0].target);
      }
    }
  }

  bool FilamentLoader::CheckTemperature() {
    constexpr auto maxTempDiff = 2.0;

    const auto& hotend = thermalManager.temp_hotend[0];

    if (hotend.target < thermalManager.extrude_min_temp) {
      popupEngine.Message(ETemp);
      return false;
    } else {
      if (hotend.celsius < hotend.target - maxTempDiff) {
        POPUP_WAITING(popupEngine, Heating) {
          thermalManager.wait_for_hotend(0);
        }
      }
      return true;
    }
  }

}



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

void Execute_GCode(const char * fmt, ...);

namespace Creality {
  class LevelingDisabler {
    public:
      static void Disable();
      static void ReEnable();
    private:
      #if HAS_LEVELING
        static bool levelingWasEnabled;
      #endif
  };

  class ManualLevel {
      static constexpr float MARGIN = 32.5; // mm
    public:
      static void ProbeBL();
      static void ProbeBR();
      static void ProbeTL();
      static void ProbeTR();
      static void ProbeC();
    private:
    private:
      ManualLevel() { }
      static ManualLevel self;
  };

  class FilamentLoader {
    public:
      static void Load();
      static void Unload();
      static void Change();
    private:
      static bool CheckTemperature();
  };
} // namespace Creality

class CrealityActions {
public:
  static void Cooldown();
  static void HomeAll();
  static void RetractProbe();

  static void HomeIfNeeded();
  static inline void HomeAndDisableLeveling() {
    HomeIfNeeded();
    Creality::LevelingDisabler::Disable();
  }
};


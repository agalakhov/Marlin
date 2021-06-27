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
 */

#include "dwin.h"
#include "geometry.h"
#include "menu.h"
#include "rotary_encoder.h"
#include "../../../libs/BL24CXX.h"
#include "../../../inc/MarlinConfigPre.h"
#include "geometry.h"

enum processID : uint8_t {
  Main, Print, Menu, Value, Option, File, Popup, Confirm, Keyboard, Wait
};

enum menuID : uint8_t {
  MainMenu,
    Prepare,
      Move,
      HomeMenu,
      ManualLevel,
      ZOffset,
      Preheat,
      ChangeFilament,
      HostActions,
    Control,
      TempMenu,
        PID,
          HotendPID,
          BedPID,
        Preheat1,
        Preheat2,
        Preheat3,
        Preheat4,
        Preheat5,
      Motion,
        HomeOffsets,
        MaxSpeed,
        MaxAcceleration,
        MaxJerk,
        Steps,
      Visual,
        ColorSettings,
      HostSettings,
        ActionCommands,
      Advanced,
        ProbeMenu,
      Info,
    Leveling,
      LevelManual,
      LevelView,
      MeshViewer,
      LevelSettings,
      ManualMesh,
      UBLMesh,
    InfoMain,
  Tune,
  PreheatHotend
};


#define Start_Process       0
#define Language_English    1
#define Language_Chinese    2


enum colorID : uint8_t {
  Default, White, Green, Cyan, Blue, Magenta, Red, Orange, Yellow, Brown, Black
};

class CrealityDWINClass {
private:
  Creality::MenuEngine menuEngine;

public:
  static constexpr size_t eeprom_data_size = 48;
  struct EEPROM_Settings { // use bit fields to save space, max 48 bytes
    bool time_format_textual : 1;
    bool beeperenable : 1;
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      uint8_t tilt_grid_size : 3;
    #endif
    uint16_t corner_pos : 10;
    uint8_t cursor_color : 4;
    uint8_t menu_split_line : 4;
    uint8_t menu_top_bg : 4;
    uint8_t menu_top_txt : 4;
    uint8_t highlight_box : 4;
    uint8_t progress_percent : 4;
    uint8_t progress_time : 4;
    uint8_t status_bar_text : 4;
    uint8_t status_area_text : 4;
    uint8_t coordinates_text : 4;
    uint8_t coordinates_split_line : 4;
    uint64_t host_action_label_1 : 48;
    uint64_t host_action_label_2 : 48;
    uint64_t host_action_label_3 : 48;
    uint64_t host_action_label_4 : 48;
    uint64_t host_action_label_5 : 48;
  } eeprom_settings;

  const char * const color_names[11] = {"Default", "White", "Green", "Cyan", "Blue", "Magenta", "Red", "Orange", "Yellow", "Brown", "Black"};
  const char * const preheat_modes[3] = {"Both", "Hotend", "Bed"};


  void Clear_Screen(uint8_t e=3);
  void Draw_Float(float value, uint8_t row, bool selected=false, uint8_t minunit=10);
  void Draw_Option(uint8_t value, const char * const * options, uint8_t row, bool selected=false, bool color=false);
  void Draw_String(char * string, uint8_t row, bool selected=false, bool below=false);
  const uint64_t Encode_String(const char * string);
  void Decode_String(const uint64_t num, char string[8]);
  const uint16_t GetColor(uint8_t color, uint16_t original, bool light=false);
  void Draw_Checkbox(uint8_t row, bool value);
  void Draw_Menu_Item(uint8_t row, DWIN::Icon icon=DWIN::NoIcon, const char * const label1=NULL, const char * const label2=NULL, bool more=false, bool centered=false);
  void Draw_Menu(uint8_t menu, uint8_t select=0, uint8_t scroll=0);
  void Redraw_Menu(bool lastprocess=true, bool lastselection=false, bool lastmenu=false);
  void Redraw_Screen();

  void Draw_Main_Menu(uint8_t select=0);
  void Print_Screen_Icons();
  void Draw_Print_Screen();
  void Draw_Print_Filename(bool reset=false);
  void Draw_Print_ProgressBar();
  void Draw_Print_ProgressRemain();
  void Draw_Print_ProgressElapsed();
  void Draw_Print_confirm();
  void Draw_SD_Item(uint8_t item, uint8_t row);
  void Draw_SD_List(bool removed=false);
  void Draw_Status_Area(bool icons=false);
  void Draw_Popup(const char * line1, const char * line2, const char * line3, uint8_t mode, DWIN::Icon icon=DWIN::NoIcon);
  void Popup_Select();
  void Update_Status_Bar(bool refresh=false);
  void Draw_Keyboard(bool restrict, bool numeric, uint8_t selected=0, bool uppercase=false, bool lock=false);
  void Draw_Keys(uint8_t index, bool selected, bool uppercase=false, bool lock=false);

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    void Draw_Bed_Mesh(int16_t selected = -1, uint8_t gridline_width = 1, uint16_t padding_x = 8, uint16_t padding_y_top = 40 + 53 - 7);
    void Set_Mesh_Viewer_Status();
  #endif

  const char * Get_Menu_Title(uint8_t menu);
  uint8_t Get_Menu_Size(uint8_t menu);
  void Menu_Item_Handler(uint8_t menu, uint8_t item, bool draw=true);


  void Popup_Handler(PopupID popupid, bool option = false);
  void Confirm_Handler(PopupID popupid);


  void Main_Menu_Control();
  void Menu_Control();
  void Value_Control();
  void Option_Control();
  void File_Control();
  void Print_Screen_Control();
  void Popup_Control();
  void Confirm_Control();
  void Keyboard_Control();


  void Setup_Value(float value, float min, float max, float unit, uint8_t type);
  void Modify_Value(float &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(uint8_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(uint16_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(int16_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(uint32_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(int8_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Option(uint8_t value, const char * const * options, uint8_t max);
  void Modify_String(char * string, uint8_t maxlength, bool restrict);


  void Update_Status(const char * const text);
  void Start_Print(bool sd);
  void Stop_Print();
  void Update();
  void State_Update();
  void Screen_Update();
  void AudioFeedback(const bool success=true);
  void Save_Settings(char *buff);
  void Load_Settings(const char *buff);
  void Reset_Settings();

public:

  // temporary functions for refactoring
  static void Tmp_PrintClicked();
  static void Tmp_PrepareClicked();
  static void Tmp_SettingsClicked();
  static void Tmp_LevelClicked();
  static void Tmp_InfoClicked();

private:
  void Draw_Time(Creality::Point position, uint16_t seconds);
};

extern CrealityDWINClass CrealityDWIN;

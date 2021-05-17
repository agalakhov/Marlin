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
#include "rotary_encoder.h"
#include "../../libs/BL24CXX.h"
#include "../../inc/MarlinConfigPre.h"
#include "geometry.h"

enum processID : uint8_t {
  Main, Print, Menu, Value, Option, File, Popup, Confirm, Wait
};

enum PopupID : uint8_t {
  Pause, Stop, Resume, SaveLevel, ETemp, ConfFilChange, PurgeMore,
  Level, Home, MoveWait, Heating,  FilLoad, FilChange, TempWarn, Runout, PIDWait, Resuming,
  FilInsert, HeaterTime, UserInput, LevelError, InvalidMesh, UI, Complete
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

#define Custom_Colors       10
#define Color_White         0xFFFF
#define Color_Light_White   0xBDD7
#define Color_Green         0x07E0
#define Color_Light_Green   0x3460
#define Color_Cyan          0x07FF
#define Color_Light_Cyan    0x04F3
#define Color_Blue          0x015F
#define Color_Light_Blue    0x3A6A
#define Color_Magenta       0xF81F
#define Color_Light_Magenta 0x9813
#define Color_Red           0xF800
#define Color_Light_Red     0x8800
#define Color_Orange        0xFA20
#define Color_Light_Orange  0xFBC0
#define Color_Yellow        0xFF0F
#define Color_Light_Yellow  0x8BE0
#define Color_Brown         0xCC27
#define Color_Light_Brown   0x6204
#define Color_Black         0x0000
#define Color_Grey          0x18E3
#define Color_Bg_Window     0x31E8  // Popup background color
#define Color_Bg_Blue       0x1125  // Dark blue background color
#define Color_Bg_Black      0x0841  // Black background color
#define Color_Bg_Red        0xF00F  // Red background color
#define Popup_Text_Color    0xD6BA  // Popup font background color
#define Line_Color          0x3A6A  // Split line color
#define Rectangle_Color     0xEE2F  // Blue square cursor color
#define Percent_Color       0xFE29  // Percentage color
#define BarFill_Color       0x10E4  // Fill color of progress bar
#define Select_Color        0x33BB  // Selected color
#define Check_Color         0x4E5C  // Check-box check color

class CrealityDWINClass {

private:
  Creality::PortraitGeometry geometry;

public:
  static constexpr size_t eeprom_data_size = 48;
  struct EEPROM_Settings { // use bit fields to save space, max 48 bytes
    bool time_format_textual : 1;
    bool beeperenable : 1;
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      uint8_t tilt_grid_size : 3;
    #endif
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
  } eeprom_settings;

  const char * const color_names[11] = {"Default", "White", "Green", "Cyan", "Blue", "Magenta", "Red", "Orange", "Yellow", "Brown", "Black"};
  const char * const preheat_modes[3] = {"Both", "Hotend", "Bed"};


  void Clear_Screen(uint8_t e=3);
  void Draw_Float(float value, uint8_t row, bool selected=false, uint8_t minunit=10);
  void Draw_Option(uint8_t value, const char * const * options, uint8_t row, bool selected=false, bool color=false);
  uint16_t GetColor(uint8_t color, uint16_t original, bool light=false);
  void Draw_Checkbox(uint8_t row, bool value);
  void Draw_Title(const char * title);
  void Draw_Menu_Item(uint8_t row, DWIN::Icon icon=DWIN::NoIcon, const char * const label1=NULL, const char * const label2=NULL, bool more=false, bool centered=false);
  void Draw_Menu(uint8_t menu, uint8_t select=0, uint8_t scroll=0);
  void Redraw_Menu(bool lastprocess=true, bool lastselection=false, bool lastmenu=false);
  void Redraw_Screen();

  void Main_Menu_Icons();
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


  void Setup_Value(float value, float min, float max, float unit, uint8_t type);
  void Modify_Value(float &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(uint8_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(uint16_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(int16_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(uint32_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Value(int8_t &value, float min, float max, float unit, void (*f)()=NULL);
  void Modify_Option(uint8_t value, const char * const * options, uint8_t max);


  void Update_Status(const char * const text);
  void Start_Print(bool sd);
  void Stop_Print();
  void Update();
  void State_Update();
  void Screen_Update();
  void AudioFeedback(const bool success=true);
  void Save_Settings(char *buff);
  void Load_Settings(const char *buff);

private:
  void Draw_Time(Creality::Point position, uint16_t seconds);
};

extern CrealityDWINClass CrealityDWIN;

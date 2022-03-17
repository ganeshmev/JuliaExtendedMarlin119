///**
// * Marlin 3D Printer Firmware
// * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
// *
// * Based on Sprinter and grbl.
// * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program.  If not, see <http://www.gnu.org/licenses/>.
// *
// */
//
///**
// * Arduino Mega with RAMPS v1.4 adjusted pin assignments
// *
// *  MKS GEN v1.3  (Extruder, Fan, Bed)
// *  MKS GEN v1.3  (Extruder, Extruder, Fan, Bed)
// *  MKS GEN v1.4  (Extruder, Fan, Bed)
// *  MKS GEN v1.4  (Extruder, Extruder, Fan, Bed)
// */
//
////#if HOTENDS > 2 || E_STEPPERS > 2
//  //#error "MKS GEN 1.3/1.4 supports up to 2 hotends / E-steppers. Comment out this line to continue."
////#endif
//
//#define BOARD_NAME "MKS GEN >= v1.3"
//
////
//// Heaters / Fans
////
//// Power outputs EFBF or EFBE
//#define MOSFET_D_PIN 7
//
////
//// PSU / SERVO
////
//// If POWER_SUPPLY is specified, always hijack Servo 3
////
//#if POWER_SUPPLY > 0
//  #define SERVO3_PIN      -1
//  #define PS_ON_PIN        4
//#endif
//
//#include "pins_RAMPS.h"
//
////
//// LCD / Controller
////
//#if ENABLED(VIKI2) || ENABLED(miniVIKI)
//  /**
//   * VIKI2 Has two groups of wires with...
//   *
//   * +Vin     + Input supply, requires 120ma for LCD and mSD card
//   * GND      Ground Pin
//   * MOSI     Data input for LCD and SD
//   * MISO     Data output for SD
//   * SCK      Clock for LCD and SD
//   * AO       Reg. Sel for LCD
//   * LCS      Chip Select for LCD
//   * SDCS     Chip Select for SD
//   * SDCD     Card Detect pin for SD
//   * ENCA     Encoder output A
//   * ENCB     Encoder output B
//   * ENCBTN   Encoder button switch
//   *
//   * BTN      Panel mounted button switch
//   * BUZZER   Piezo buzzer
//   * BLUE-LED Blue LED ring pin (3 to 5v, mosfet buffered)
//   * RED-LED  Red LED ring pin (3 to 5v, mosfet buffered)
//   *
//   * This configuration uses the following arrangement:
//   *
//   * EXP1 D37 = EN2   D35 = EN1     EXP2 D50 = MISO  D52 = SCK
//   *      D17 = BLUE  D16 = RED          D31 = ENC   D53 = SDCS
//   *      D23 = KILL  D25 = BUZZ         D33 = ---   D51 = MOSI
//   *      D27 = A0    D29 = LCS          D49 = SDCD  RST = ---
//   *      GND = GND   5V  = 5V           GND = ---   D41 = ---
//   */
//
//  #undef BTN_EN1
//  #undef BTN_EN2
//  #undef BTN_ENC
//  #undef DOGLCD_A0
//  #undef DOGLCD_CS
//  #undef SD_DETECT_PIN
//  #undef BEEPER_PIN
//  #undef KILL_PIN
//
//  //
//  // VIKI2 12-wire lead
//  //
//
//  // orange/white         SDCD
//  #define SD_DETECT_PIN   49
//
//  // white                ENCA
//  #define BTN_EN1         35
//
//  // green                ENCB
//  #define BTN_EN2         37
//
//  // purple               ENCBTN
//  #define BTN_ENC         31
//
//  // brown                A0
//  #define DOGLCD_A0       27
//
//  // green/white          LCS
//  #define DOGLCD_CS       29
//
//                       // 50    gray   MISO
//                       // 51    yellow MOSI
//                       // 52    orange SCK
//
//  // blue                 SDCS
//  //#define SDSS            53
//
//  //
//  // VIKI2 4-wire lead
//  //
//
//  // blue                 BTN
//  #define KILL_PIN        23
//
//  // green                BUZZER
//  #define BEEPER_PIN      25
//
//  // yellow               RED-LED
//  #define STAT_LED_RED_PIN 16
//
//  // white                BLUE-LED
//  #define STAT_LED_BLUE_PIN 17
//
//#endif


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
 * MKS GEN L V2 – Arduino Mega2560 with RAMPS v1.4 pin assignments
 */

// #if HOTENDS > 2 || E_STEPPERS > 2
//   #error "MKS GEN L V2.1 supports up to 2 hotends / E-steppers. Comment out this line to continue."
// #endif

#define BOARD_INFO_NAME "MKS GEN L V2.1"

//
// Heaters / Fans
//
// Power outputs EFBF or EFBE
#define MOSFET_D_PIN                           7

//
// CS Pins wired to avoid conflict with the LCD
// See https://www.thingiverse.com/asset:66604
//

#ifndef X_CS_PIN
  #define X_CS_PIN                            63
#endif
#ifndef Y_CS_PIN
  #define Y_CS_PIN                            64
#endif
#ifndef Z_CS_PIN
  #define Z_CS_PIN                            65
#endif
#ifndef E0_CS_PIN
  #define E0_CS_PIN                           66
#endif
#ifndef E1_CS_PIN
  #define E1_CS_PIN                           12
#endif

// TMC2130 Diag Pins (currently just for reference)
#define X_DIAG_PIN                             3
#define Y_DIAG_PIN                            14
#define Z_DIAG_PIN                            18
#define E0_DIAG_PIN                            2
#define E1_DIAG_PIN                           15

#ifndef SERVO1_PIN
  #define SERVO1_PIN                          21
#endif
#ifndef SERVO2_PIN
  #define SERVO2_PIN                          39
#endif
#ifndef SERVO3_PIN
  #define SERVO3_PIN                          32
#endif

#ifndef E1_SERIAL_TX_PIN
  #define E1_SERIAL_TX_PIN                    20
#endif
#ifndef E1_SERIAL_RX_PIN
  #define E1_SERIAL_RX_PIN                    12
#endif

#include "pins_RAMPS.h"

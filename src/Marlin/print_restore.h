/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * print_restore.h - Resume an SD print after board reset
 */

#ifndef _PRINT_RESTORE_H_
#define _PRINT_RESTORE_H_

#include "cardreader.h"
#include "types.h"
#include "MarlinConfig.h"

//#define DEBUG_PRINT_RESTORE


typedef struct {
  uint8_t valid_head;

  // Machine state
  float current_position[NUM_AXIS], feedrate_percentage;
  int16_t target_temperature[HOTENDS],
          fanSpeeds[FAN_COUNT];

  // #if HAS_HEATED_BED
  int16_t target_temperature_bed;
  // #endif

  #if HAS_LEVELING
    bool leveling;
    float fade;
  #endif

  // #if ENABLED(BABYSTEPPING)
  // 	float babystep = 0;
  // #endif

  // Command queue
  uint8_t cmd_queue_index_r, commands_in_queue;
  char command_queue[BUFSIZE][MAX_CMD_SIZE];

  // Print job name
  char sd_filename[MAXPATHNAMELENGTH];

  // Print file position
  uint32_t sdpos;

  // Job elapsed time
  millis_t print_job_elapsed;

  uint8_t valid_foot;

} print_restore_info_t;

extern print_restore_info_t print_restore_info;

enum PrintRestorePhase : unsigned char {
  IDLE,
  BIN_FOUND,
  FILE_MADE,
  LCD_MAYBE,
  START
};
extern PrintRestorePhase print_restore_phase;

void do_print_restore();
void save_print_restore_info();

#endif // _PRINT_RESTORE_H_

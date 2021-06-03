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
 * print_restore.cpp - Resume an SD print after power-loss
 */

#include "MarlinConfig.h"

#if ENABLED(PRINT_RESTORE)

#include "print_restore.h"

#include "cardreader.h"
#include "planner.h"
#include "printcounter.h"
#include "serial.h"
#include "temperature.h"
#include "ultralcd.h"
#include "print_restore_util.h"

// Recovery data
print_restore_info_t print_restore_info;
PrintRestorePhase print_restore_phase = IDLE;

// Extern
extern uint8_t commands_in_queue, cmd_queue_index_r;
extern char command_queue[BUFSIZE][MAX_CMD_SIZE];

// #if ENABLED(BABYSTEPPING)
// 	extern float babystep_total;
// #endif

// Private
// static char sd_filename[MAXPATHNAMELENGTH];
#define DEBUG_PRINT_RESTORE
#if ENABLED(DEBUG_PRINT_RESTORE)
  void debug_print_job_recovery() {
    SERIAL_PROTOCOLLNPGM("");
    SERIAL_PROTOCOLLNPGM("Print Restore Debug Start");
    SERIAL_PROTOCOLPAIR("valid_head:", (int)print_restore_info.valid_head);
    SERIAL_PROTOCOLLNPAIR(" valid_foot:", (int)print_restore_info.valid_foot);

    if (print_restore_info.valid_head) {
      if (print_restore_info.valid_head == print_restore_info.valid_foot) {

        SERIAL_PROTOCOLPGM("current_position");
        LOOP_XYZE(i) SERIAL_PROTOCOLPAIR(": ", print_restore_info.current_position[i]);
        SERIAL_EOL();

        SERIAL_PROTOCOLLNPAIR("feedrate %: ", print_restore_info.feedrate_percentage);

        SERIAL_PROTOCOLPGM("target_temperature");
        HOTEND_LOOP() SERIAL_PROTOCOLPAIR(": ", print_restore_info.target_temperature[e]);
        SERIAL_EOL();

        SERIAL_PROTOCOLPGM("fanSpeeds");
        for(uint8_t i = 0; i < FAN_COUNT; i++) SERIAL_PROTOCOLPAIR(": ", print_restore_info.fanSpeeds[i]);
        SERIAL_EOL();

        #if HAS_HEATED_BED
          SERIAL_PROTOCOLLNPAIR("target_temperature_bed: ", print_restore_info.target_temperature_bed);
        #endif

        #if HAS_LEVELING
          SERIAL_PROTOCOLPAIR("leveling: ", int(print_restore_info.leveling));
          SERIAL_PROTOCOLLNPAIR(" fade: ", int(print_restore_info.fade));
        #endif

        // #if ENABLED(BABYSTEPPING)
        // 	SERIAL_PROTOCOLLNPAIR_F("babystepping: ", print_restore_info.babystep);
        // #endif
        
        SERIAL_PROTOCOLLNPAIR("cmd_queue_index_r: ", print_restore_info.cmd_queue_index_r);
        SERIAL_PROTOCOLLNPAIR("commands_in_queue: ", print_restore_info.commands_in_queue);

        // if (recovery)
        //   for (uint8_t i = 0; i < job_recovery_commands_count; i++) SERIAL_PROTOCOLLNPAIR("> ", job_recovery_commands[i]);
        // else
        //   for (uint8_t i = 0; i < print_restore_info.commands_in_queue; i++) SERIAL_PROTOCOLLNPAIR("> ", print_restore_info.command_queue[i]);
        
        uint8_t r = print_restore_info.cmd_queue_index_r;
        while (print_restore_info.commands_in_queue) {
          SERIAL_PROTOCOLLNPAIR("> ", print_restore_info.command_queue[r]);

          print_restore_info.commands_in_queue--;
          r = (r + 1) % BUFSIZE;
        }

        SERIAL_PROTOCOLLNPAIR("sd_filename: ", print_restore_info.sd_filename);
        SERIAL_PROTOCOLLNPAIR("sdpos: ", print_restore_info.sdpos);
        SERIAL_PROTOCOLLNPAIR("print_job_elapsed: ", print_restore_info.print_job_elapsed);

      } else {
        SERIAL_PROTOCOLLNPGM("INVALID DATA");
      }
    }
    SERIAL_PROTOCOLLNPGM("Print Restore Debug End");
    SERIAL_PROTOCOLLNPGM("");
  }
#endif // DEBUG_PRINT_RESTORE

/**
 * Check for Print Job Recovery
 * If the file has a saved state, populate the job_recovery_commands queue
 */
void do_print_restore() {
  //if (job_recovery_commands_count > 0) return;
  memset(&print_restore_info, 0, sizeof(print_restore_info));
  //ZERO(job_recovery_commands);
  
  SERIAL_PROTOCOLLNPAIR("Print Restore Gcode file: ", CardReader::PrintRestoreGcodeFilename);
  
  if (!card.cardOK) card.initsd();

  if (card.cardOK) {

    #if ENABLED(DEBUG_PRINT_RESTORE)
      SERIAL_PROTOCOLLNPAIR("Init job recovery info. Size: ", (int) sizeof(print_restore_info));
    #endif

    if (card.printRestoreBinExists()) {
      print_restore_phase = BIN_FOUND;
      
      card.openPrintRestoreBin(true);
      card.loadPrintRestoreInfo();
      card.closePrintRestoreBin();
      // card.removePrintRestoreBin();

      #if ENABLED(DEBUG_PRINT_RESTORE)
        SERIAL_PROTOCOLLNPGM("Loaded print_restore_info");
        debug_print_job_recovery();
      #endif

      if (print_restore_info.valid_head && print_restore_info.valid_head == print_restore_info.valid_foot) {
        char cmd[40], temp[16];
        
        card.openFile(CardReader::PrintRestoreGcodeFilename, false);
        
        // Restore temperatures
        #if HAS_HEATED_BED
          // Restore the bed temperature
          sprintf_P(cmd, PSTR("M140 S%i"), print_restore_info.target_temperature_bed);
          card.write_command(cmd);
        #endif

        // Restore all hotend temperatures
        HOTEND_LOOP() {
          sprintf_P(cmd, PSTR("M104 S%i"), print_restore_info.target_temperature[e]);
          card.write_command(cmd);
        }

        // Wait for temperatures
        #if HAS_HEATED_BED
          // Wait for bed temperature
          sprintf_P(cmd, PSTR("M190 S%i"), print_restore_info.target_temperature_bed);
          card.write_command(cmd);
        #endif

        // Wait for all hotend temperatures
        HOTEND_LOOP() {
          sprintf_P(cmd, PSTR("M109 S%i"), print_restore_info.target_temperature[e]);
          card.write_command(cmd);
        }

        // Restore print cooling fan speeds
        for (uint8_t i = 0; i < FAN_COUNT; i++) {
          sprintf_P(cmd, PSTR("M106 P%i S%i"), i, print_restore_info.fanSpeeds[i]);
          card.write_command(cmd);
        }
        
        // show message
        sprintf_P(cmd, PSTR("M117 Restarting %s"), print_restore_info.sd_filename);
        card.write_command(cmd);
          
        #if HAS_LEVELING
          // Leveling off before G92 or G28
          strcpy_P(cmd, PSTR("M420 S0 Z0"));            
          card.write_command(cmd);
        #endif
        
        // Home axes
        strcpy_P(cmd, PSTR("G28"));
        card.write_command(cmd);
        
        #if HAS_LEVELING
          // Leveling on
          strcpy_P(cmd, PSTR("M420 S1"));               
          card.write_command(cmd);
        #endif
        
        // go to Z + 5 mm
        fmtSaveLine(cmd, PSTR("G1 Z%s"), ftostr33s, print_restore_info.current_position[Z_AXIS] + 5);
        card.write_command(cmd);
        
        // set extruder position to zero
        strcpy_P(cmd, PSTR("G92 E0"));
        card.write_command(cmd);
        
        // extrude 5 mm
        strcpy_P(cmd, PSTR("G1 F200 E5"));
        card.write_command(cmd);
        
        // set extruder position to 5 mm
        strcpy_P(cmd, PSTR("G92 F200 E5"));
        card.write_command(cmd);
        
        // set extruder position to restored position
        fmtSaveLine(cmd, PSTR("G92 E%s"), ftostr53s, print_restore_info.current_position[E_AXIS]);
        card.write_command(cmd);
        
        // restore X axis
        fmtSaveLine(cmd, PSTR("G1 F1200 X%s"), ftostr33s, print_restore_info.current_position[X_AXIS]);
        card.write_command(cmd);
        
        // restore Y axis
        fmtSaveLine(cmd, PSTR("G1 Y%s"), ftostr33s, print_restore_info.current_position[Y_AXIS]);
        card.write_command(cmd);
        
        // restore Z axis
        fmtSaveLine(cmd, PSTR("G1 Z%s"), ftostr33s, print_restore_info.current_position[Z_AXIS]);
        card.write_command(cmd);

        // restore feedrate
        fmtSaveLine(cmd, PSTR("M220 S%s"), ftostr33s, print_restore_info.feedrate_percentage);
        card.write_command(cmd);

        // restore babystep
        // #if ENABLED(BABYSTEPPING)
        // 	if (print_restore_info.babystep != 0) {
        // 		fmtSaveLine(cmd, PSTR("M290 Z%s"), ftostr33s, print_restore_info.babystep);
        // 		card.write_command(cmd);
        // 	}
        // #endif
        
        // buffered commands
        uint8_t r = print_restore_info.cmd_queue_index_r;
        while (print_restore_info.commands_in_queue) {
          strcpy(cmd, print_restore_info.command_queue[r]);
          card.write_command(cmd);
          
          print_restore_info.commands_in_queue--;
          r = (r + 1) % BUFSIZE;
        }
        
        // select original file
        sprintf_P(cmd, PSTR("M23 %s"), print_restore_info.sd_filename);
        card.write_command(cmd);
        
        // set file offset
        sprintf_P(cmd, PSTR("M26 S%lu"), print_restore_info.sdpos);
        card.write_command(cmd);
        
        // start print
        strcpy_P(cmd, PSTR("M24"));
        card.write_command(cmd);
        
        // show print status
        strcpy_P(cmd, PSTR("M27"));
        card.write_command(cmd);
        
        card.closefile();
        
        print_restore_phase = FILE_MADE;	
        
        memset(&print_restore_info, 0, sizeof(print_restore_info));
        
        // feedrate_percentage = 100;
        
        return;
        
      } else {
        if (print_restore_info.valid_head != print_restore_info.valid_foot)
          LCD_ALERTMESSAGEPGM("INVALID RESTORE BIN");
        memset(&print_restore_info, 0, sizeof(print_restore_info));
      }
    }
  }
  
  print_restore_phase = IDLE;
}


/**
 * Save the current machine state to the "bin" file
 */
void save_print_restore_info() {
  card.openPrintRestoreBin(false);
  
  if (print_restore_phase != IDLE) return;
  
  #if SAVE_INFO_INTERVAL_MS > 0
    next_save_ms = ms + SAVE_INFO_INTERVAL_MS;
  #endif

  // Head and foot will match if valid data was saved
  if (!++print_restore_info.valid_head) ++print_restore_info.valid_head; // non-zero in sequence
  print_restore_info.valid_foot = print_restore_info.valid_head;

  // Machine state
  COPY(print_restore_info.current_position, current_position);
  print_restore_info.feedrate_percentage = feedrate_percentage;
  COPY(print_restore_info.target_temperature, thermalManager.target_temperature);
  #if HAS_HEATED_BED
    print_restore_info.target_temperature_bed = thermalManager.target_temperature_bed;
  #endif
  COPY(print_restore_info.fanSpeeds, fanSpeeds);

  #if HAS_LEVELING
    print_restore_info.leveling = planner.leveling_active;
    print_restore_info.fade = (
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        planner.z_fade_height
      #else
        0
      #endif
    );
  #endif

  // #if ENABLED(BABYSTEPPING)
  // 	print_restore_info.babystep = babystep_total;
  // 	// SERIAL_PROTOCOLLNPAIR("Babystep: ", babystep_total);
  // #endif

  // Commands in the queue
  print_restore_info.cmd_queue_index_r = cmd_queue_index_r;
  print_restore_info.commands_in_queue = commands_in_queue;
  
  COPY(print_restore_info.command_queue, command_queue);

  // Elapsed print job time
  print_restore_info.print_job_elapsed = print_job_timer.duration() * 1000UL;

  // SD file position
  card.getAbsFilename(print_restore_info.sd_filename);
  print_restore_info.sdpos = card.getIndex();

  // #if ENABLED(DEBUG_PRINT_RESTORE)
  // 	SERIAL_PROTOCOLLNPGM("Saving print_restore_info");
  // 	debug_print_job_recovery(false);
  // #endif

  card.savePrintRestoreInfo();
}

#endif // PRINT_RESTORE

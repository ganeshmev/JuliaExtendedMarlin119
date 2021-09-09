#ifndef FWCONFIG_H
#define FWCONFIG_H

//===========================================================================
//========================= Fracktal Works Julia 2018 =======================
//===========================================================================

#include "_FracktalWorks.h"

#ifndef VARIANT
  #error "Variant not defined"
#endif

/**  Machine Branding  **/
// #define USE_AUTOMATIC_VERSIONING

/** Machine name for variant **/
#if BV(JULIA_2018_GLCD)
  #define CUSTOM_MACHINE_NAME "Julia Basic"
#elif BV(JULIA_2018_GLCD_HB)
  #define CUSTOM_MACHINE_NAME "Julia Intermediate"
#elif BV(JULIA_2018_RPI)
  #define CUSTOM_MACHINE_NAME "Julia Advanced"
#elif BV(JULIA_2018_RPI_E)
  #define CUSTOM_MACHINE_NAME "Julia Extended"
#elif BV(JULIA_2018_PRO_SINGLE)
  #define CUSTOM_MACHINE_NAME "Julia Pro Single"
#elif BV(JULIA_2018_PRO_DUAL)
  #define CUSTOM_MACHINE_NAME "Julia Pro Dual"
#elif BV(JULIA_2018_PRO_SINGLE_A)
  #define CUSTOM_MACHINE_NAME "Julia Pro Single ABL"
#elif BV(JULIA_2018_PRO_DUAL_A)
  #define CUSTOM_MACHINE_NAME "Julia Pro Dual ABL"
#endif

#define STRING_CONFIG_H_AUTHOR  "Fracktal Works"
#define STRING_SPLASH_LINE1     MSG_MARLIN " " MARLIN_BRANCH
#define STRING_SPLASH_LINE2     WEBSITE_URL


/**  Control board  **/
#define MOTHERBOARD   BOARD_MKS_GEN_13 //BOARD_MKS_13 //BOARD_RAMPS_13_EFB
#define BAUDRATE      115200

/**  Filament  **/
#define DEFAULT_NOMINAL_FILAMENT_DIA  1.75


/**  Kinematics  **/
#define COREXY


/**  Bed dimensions  **/
#if BV_REG()
  #define X_BED_SIZE  200
  #define Y_BED_SIZE  200
  #define Z_MAX_POS   210
#elif BV(JULIA_2018_RPI_E)  
  #define X_BED_SIZE  250
  #define Y_BED_SIZE  250
  #define Z_MAX_POS   320
#elif BV(JULIA_2018_PRO_SINGLE)
  #define X_BED_SIZE  400
  #define Y_BED_SIZE  400
  #define Z_MAX_POS   400
#elif BV(JULIA_2018_PRO_DUAL)
  #define X_BED_SIZE  370
  #define Y_BED_SIZE  395
  #define Z_MAX_POS   400
#elif BV(JULIA_2018_PRO_SINGLE_A) || BV(JULIA_2018_PRO_SINGLE_A24)
  #define X_BED_SIZE  400
  #define Y_BED_SIZE  400
  #define Z_MAX_POS   400
#elif BV(JULIA_2018_PRO_DUAL_A) || BV(JULIA_2018_PRO_DUAL_A24)
  #define X_BED_SIZE  370
  #define Y_BED_SIZE  395
  #define Z_MAX_POS   400
#endif

/** Min Pos **/
// #if BV(JULIA_2018_PRO_SINGLE)  // BV_PRO_SINGLE()
//   // PEI (Left, Front) at (-20, -10): (5, 16)
//   #define X_MIN_POS -25	// -40
//   #define Y_MIN_POS -40 // -10
//   #define Z_MIN_POS 0
// #elif BV(JULIA_2018_PRO_DUAL)  // BV_PRO_DUAL()
//   #define X_MIN_POS 0 //-40 
//   #define Y_MIN_POS 0 //-10
//   #define Z_MIN_POS 0
// #else
//   #define X_MIN_POS 0
//   #define Y_MIN_POS 0
//   #define Z_MIN_POS 0
// #endif
#define X_MIN_POS 0
#define Y_MIN_POS 0
#define Z_MIN_POS 0

/**  Stepper  **/
#define X_DRIVER_TYPE     TMC2208
#define Y_DRIVER_TYPE     TMC2208
#define Z_DRIVER_TYPE     TMC2208
#define E0_DRIVER_TYPE    TMC2208
#define E1_DRIVER_TYPE    TMC2208

#if BV_PRO() || BV_PRO_ABL() || BV_PRO_ABL24()
  #define INVERT_X_DIR    true
  #define INVERT_Y_DIR    true
  #define INVERT_Z_DIR    true

  #define INVERT_E0_DIR   false
  #define INVERT_E1_DIR   true
#else
  #define INVERT_X_DIR    true//false
  #define INVERT_Y_DIR    true//false
  #define INVERT_Z_DIR    true//false

  #define INVERT_E0_DIR   false//true
#endif

/**  Enstops  **/
#define USE_XMIN_PLUG
#define USE_YMAX_PLUG
#define USE_ZMAX_PLUG

/**  Homing  **/
#define X_HOME_DIR  -1
#define Y_HOME_DIR   1
#define Z_HOME_DIR   1

#if BV_REG() || BV(JULIA_2018_RPI_E)
  #define MANUAL_X_HOME_POS -10
  #define MANUAL_Y_HOME_POS Y_BED_SIZE
  #define MANUAL_Z_HOME_POS Z_MAX_POS
#elif BV_PRO_SINGLE()
  #define MANUAL_X_HOME_POS -20
  #define MANUAL_Y_HOME_POS Y_BED_SIZE
  #define MANUAL_Z_HOME_POS Z_MAX_POS
#elif BV_PRO_DUAL()
  #define MANUAL_X_HOME_POS -40 // (-1 * HOTEND_OFFSET_X[1])
  #define MANUAL_Y_HOME_POS Y_BED_SIZE
  #define MANUAL_Z_HOME_POS Z_MAX_POS
#endif

#define HOMING_FEEDRATE_XY  (50*60)
#define HOMING_FEEDRATE_Z   (20*60)

/**  Movement  **/
#define S_CURVE_ACCELERATION
#if BV_PRO_ABL24()
  #define DEFAULT_AXIS_STEPS_PER_UNIT   { 100,  100, 503.937, 140 }
#else
  #define DEFAULT_AXIS_STEPS_PER_UNIT   { 100,  100, 503.937, 140 }
#endif
#if BV_PRO() || BV_PRO_ABL() || BV_PRO_ABL24()
  #define DEFAULT_MAX_FEEDRATE          { 300, 300, 20, 45 }
#else
  #define DEFAULT_MAX_FEEDRATE          { 300, 300, 20, 45 }
#endif
#if BV_PRO() || BV_PRO_ABL()
  #define DEFAULT_MAX_ACCELERATION      { 600, 500, 50, 10000 }
  #define DEFAULT_ACCELERATION          400    // X, Y, Z and E acceleration for printing moves
#elif BV_PRO_ABL24()
  #define DEFAULT_MAX_ACCELERATION      { 1500, 1000, 50, 10000 }
  #define DEFAULT_ACCELERATION          1000    // X, Y, Z and E acceleration for printing moves
#else
  #define DEFAULT_MAX_ACCELERATION      { 2000, 2000, 50, 10000 }
  #define DEFAULT_ACCELERATION          1500    // X, Y, Z and E acceleration for printing moves
#endif
#define DEFAULT_RETRACT_ACCELERATION  2000    // E acceleration for retracts
#define DEFAULT_TRAVEL_ACCELERATION   1500    // X, Y, Z acceleration for travel (non printing) moves
#define DEFAULT_XJERK                 10.0
#define DEFAULT_YJERK                 10.0
#define DEFAULT_ZJERK                 0.4
#define DEFAULT_EJERK                 10.0

/** Extruder Count **/
#if BV_PRO_DUAL()
  #define EXTRUDERS 2
#else
  #define EXTRUDERS 1
#endif

/*** Dual Nozzle ***/
#if BV_PRO_DUAL()
  #define SWITCHING_NOZZLE
  #define SWITCHING_NOZZLE_SERVO_NR 0
  #define SWITCHING_NOZZLE_SERVO_ANGLES { 0, 180 }   // Angles for E0, E1

  #define NUM_SERVOS 1
  #define DEACTIVATE_SERVOS_AFTER_MOVE

  #define HOTEND_OFFSET_X {0.0, 36.0}
  #define HOTEND_OFFSET_Y {0.0,  0.0}
  #define HOTEND_OFFSET_Z {0.0, -4.0}
#endif

/**  Thermistor  **/
#define TEMP_SENSOR_0       3
#if BV_PRO_DUAL()
  #define TEMP_SENSOR_1     3
#else
  #define TEMP_SENSOR_1     0
#endif
#if BV(JULIA_2018_GLCD)
  #define TEMP_SENSOR_BED   0
#else
  #define TEMP_SENSOR_BED   3
#endif

/**  Thermal  **/
#define TEMP_RESIDENCY_TIME     1
#define TEMP_HYSTERESIS         4
#define TEMP_BED_RESIDENCY_TIME 2

/**  PID temperature settings  **/
#define  DEFAULT_Kp   67.13 // 42.96
#define  DEFAULT_Ki   10.75 // 5.14
#define  DEFAULT_Kd   104.85 // 89.73

/**  Thermal Runaway  **/
#if NBV(JULIA_2018_GLCD)
  #define THERMAL_PROTECTION_BED
#endif


/**  Bed leveling  **/
#if BV_PRO_ABL() || BV_PRO_ABL24() || BV(JULIA_2018_RPI_E)   // auto bed leveling
  #define Z_MIN_PROBE_ENDSTOP
  #define FIX_MOUNTED_PROBE
  #define Z_PROBE_OFFSET_FROM_EXTRUDER 0.2   // Z offset: -below +above  [the nozzle]

  #define DELAY_BEFORE_PROBING 1000   // (ms) To prevent vibrations from triggering sensor

  #define MIN_PROBE_EDGE 25
  #define Z_PROBE_SPEED_FAST (HOMING_FEEDRATE_Z/10)
  #define Z_PROBE_SPEED_SLOW (Z_PROBE_SPEED_FAST / 15)
  #define MULTIPLE_PROBING 2

  #define Z_CLEARANCE_DEPLOY_PROBE   5 // Z Clearance for Deploy/Stow
  #define Z_CLEARANCE_BETWEEN_PROBES  3 // Z Clearance between probe points
  #define Z_CLEARANCE_MULTI_PROBE     1 // Z Clearance between multiple probes

  #define Z_MIN_PROBE_REPEATABILITY_TEST

  #define AUTO_BED_LEVELING_BILINEAR
  #define DEBUG_LEVELING_FEATURE
#else   // mesh bed leveling
  #define MESH_BED_LEVELING
  #if  BV_PRO()
    #define GRID_MAX_POINTS_X 3
  #else
    #define GRID_MAX_POINTS_X 2
  #endif
  #if BV_NPI()
    #define LCD_BED_LEVELING
  #endif
#endif


/**  MKS-MINI12864 LCD  **/
#if BV_NPI()
  #define SDSUPPORT
  #define ENCODER_PULSES_PER_STEP 2
  #define ENCODER_STEPS_PER_MENU_ITEM 2
  #define SPEAKER
  #define MKS_MINI_12864

  #define SHOW_BOOTSCREEN
  #define SHOW_CUSTOM_BOOTSCREEN
#endif


/**  Additional features  **/
#define EEPROM_SETTINGS     // Enable for M500 and M501 commands
#define NOZZLE_PARK_FEATURE
#define PRINTCOUNTER
#if BV_NPI()
  #define PRINT_RESTORE  // print restore
  #define FW_BABYSTEP
#endif

// Remove Fan 1
#ifdef FAN1_PIN
	#undef FAN1_PIN
#endif
#define FAN1_PIN -1
//===========================================================================
//===================== End of Fracktal Works Julia 2018 ====================
//===========================================================================

#endif

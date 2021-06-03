#ifndef _PRINT_RESTORE_UTIL_H
#define _PRINT_RESTORE_UTIL_H

	// Convert float to string with 123.4 format, signed
	void ftostr31s(float x, char c[8]);

	// Convert float to string with 123.4 format, unsigned
	void ftostr31u(float x, char c[8]);

	// Convert float to string with 123.456 format, signed
	void ftostr33s(float x, char c[8]);
	  
	// Convert float to string with 12345.678 format, signed
	void ftostr53s(float x, char c[8]);


	// fill command placeholder using float converter and value
	void fmtSaveLine(char *output, const char *placeholder, void (*converter)(float, char[10]), float value);
	
	
#endif
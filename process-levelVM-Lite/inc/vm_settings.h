/* You May Edit This File
 * - vm_settings.h (KAIA VM)
 * - Copyright: Prof. Kevin Andrea, George Mason University.  All Rights Reserved
 * - Date: Jan 2022
 */

#ifndef VM_SETTINGS_H
#define VM_SETTINGS_H

// This is the Prompt Formatting for the Shell
#define PROMPT "[KAIA-VM]$ "

// Set USE_COLORS to 1 for colors or 0 for normal.
#define USE_COLORS 1

// Turns Debug printing on (1) and off (0)
#define DEFAULT_DEBUG 0

// Time to run each Process for between Context Switching
#define SLEEP_USEC    250000 //  250ms
// Time to wait between Context Switches before Running Next Process
#define BETWEEN_USEC 1000000 // 1000000 = 1000ms = 1 sec


//////////////////////////////////////////////////////////////////////
//  Do not modify anything below this line. 
//////////////////////////////////////////////////////////////////////
#define LOCAL_CMDS_ONLY 0 // Restricts Shell to local folder binaries only (recompile lib on change)
#define MAX_CMD_LINE 256 // Max characters in a user input
#define MAX_STATUS   512 // Max characters in a status message
#define MAX_PROC 64  // Max Processes Runnable
#define MAX_CMD  256 // Max size of a single command
#define MAX_PATH 512 // Max size of a command with full absolute path
#define MAX_ARGS 16  // Max number of args for a single shell command

#endif

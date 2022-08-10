/* FEEL FREE TO EDIT THIS FILE
 * - test_schedule.c (KAIA VM)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
// Local Includes
#include "vm_support.h" // Gives abort_error, print_warning, print_status, print_debug commands
#include "vm_schedule.h" // Your header for the functions you're testing.

int debug_mode = 1; // Hardcodes debug on for the custom print functions

// Local Prototypes
void test_scheduler_create();

int main() {
  print_status("Test 1: Testing Scheduler Create");
  test_scheduler_create();

  return 0;
}

void test_scheduler_create() {
  print_debug("...Calling schedule_create()");
  schedule_header_t *header = schedule_create();
  if(header == NULL) {
    abort_error("...schedule_create returned NULL!", __FILE__);
  }
  if(header->ready_queue == NULL || header->suspended_queue == NULL || header->defunct_queue == NULL) {
    abort_error("...schedule_create returned at least one NULL queue.", __FILE__);
  }
  if(header->ready_queue->count != 0) {
    print_warning("...The count on Ready Queue is not 0!");
  }

  print_status("...Printing the Schedule");
  print_schedule_debug(header);
  print_status("...schedule_create is looking good so far.");
}

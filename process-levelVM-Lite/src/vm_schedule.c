/* This is the only file you will be editing.
 * - Copyright of Starter Code: Prof. Kevin Andrea, George Mason University.  All Rights Reserved
 * - Copyright of Student Code: Weilon Price
 * - Date: Jan 2022
 */

/* 
 * Name: Weilon Price
 * GMU CS student
 */

// System Includes
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>
// Local Includes
#include "vm_schedule.h"
#include "vm_support.h"

/* Feel free to create any helper functions you like! */

/* Below are Helper functions */

int set_flags(int flags, int state){
	if(state == 31){
	flags = flags & ~(1 << 30);
        flags = flags & ~(1 << 29);
        flags = flags | (1 << 31);
	return flags;
	}

	else if(state == 30){
	flags = flags & ~(1 << 31);
        flags = flags & ~(1 << 29);
        flags = flags | (1 << 30);
        return flags;
	}

	else if(state == 29){
	flags = flags & ~(1 << 31);
        flags = flags & ~(1 << 30);
        flags = flags | (1 << 29);
        return flags;
	}

	return flags;
}

/* insert process into last place of queue */
queue_header_t* insert_last(queue_header_t *queue, process_node_t *process){
	
	if(queue == NULL || process == NULL){exit(1);}	
	
	/* head is empty, process becomes head */
	if(queue->head == NULL){
		queue->head = process;
		process->next = NULL;
		queue->count += 1;
		return queue;
	}

	/* loops to end of queue, and add process at the end */
	else{
		process_node_t *current = queue->head;
		while(current->next != NULL){current = current->next;}
		current->next = process;
		process->next = NULL;
		queue->count += 1;
		return queue;
	}
	return queue;
}

/* find and remove pid from queue, returns a process node with matching pid */

process_node_t* find_remove_pid(process_node_t **head, pid_t pid){
	
	if(*head == NULL){exit(1);}

	process_node_t *current = *head;
	process_node_t *prev = NULL;

	/* checking if head pid equals input pid */
        if(current->pid == pid){
		*head = (*head)->next;
		return current;
        }

	/* searching rest of queue to find matching pid */
        else{
                prev = current;
                current = current->next;

                while(current != NULL){
                        if(current->pid == pid){
                                prev->next = current->next;
                        	return current;
			}
                        else{
                                prev = current;
                                current = current->next;
                        }
                }
        }
	return NULL;
}

/* inserts node into queue in ascending pid order, returns a queue */
queue_header_t* insert_ascending(queue_header_t *queue, process_node_t *process){
	
	if(queue == NULL || process == NULL){return NULL;}

	process_node_t *head = queue->head;
	
	/* if head is null then process = head */
	if(head == NULL){
		queue->head = process;
		queue->count += 1;
		queue->head->next = NULL;
		return queue;
	}

	/* if head is only node in queue, then process either go in front or behind head */
	else if(queue->count == 1){
		if(process->pid < head->pid){
			process->next = queue->head;
			queue->head = process;
			queue->count += 1;

			return queue;
		}
		else{
			head->next = process;
			process->next = NULL;
			queue->count += 1;
			return queue;
		}
	}

	/* go through entire queue to find the right spot */
	else{
		head = head->next;

		while(head != NULL){
			if(process->pid > head->pid && process->pid < head->next->pid){
				process->next = head->next;
				head->next = process;
				queue->count += 1;
				return queue;
			}
			/* reached end of queue, puts node at the end */
			if(head->next == NULL){
				head->next = process;
				queue->count += 1;
				return queue;
			}
		}
	}
	return queue;
}

/* end of helper functions */


/* Initialize the schedule_header_t Struct
 * Follow the specification for this function.
 * Returns a pointer to the new schedule_header_t or NULL on any error.
 */
schedule_header_t *schedule_create() {

	/* initialize Schedule Header */
	schedule_header_t *schedule_header = malloc(sizeof(schedule_header_t));
	if (schedule_header == NULL) {return NULL;}

	/* initialize the three queues with error checking */
	schedule_header->ready_queue = malloc(sizeof(queue_header_t));
	if(schedule_header->ready_queue == NULL) {return NULL;}

	schedule_header->suspended_queue = malloc(sizeof(queue_header_t));
	if(schedule_header->suspended_queue == NULL) {return NULL;}

	schedule_header->defunct_queue = malloc(sizeof(queue_header_t));
	if(schedule_header->defunct_queue == NULL) {return NULL;}
	
	/* pointer of head set to NULL and count set to 0 */
	schedule_header->ready_queue->head = NULL;
	schedule_header->ready_queue->count = 0;
	
	schedule_header->suspended_queue->head = NULL;
        schedule_header->suspended_queue->count = 0;

	schedule_header->defunct_queue->head = NULL;
	schedule_header->defunct_queue->count = 0;

	return schedule_header;
}

/* Adds a process into the appropriate singly linked list.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int schedule_insert(schedule_header_t *header, process_node_t *process) {
	
	/* error checking on input */
	if(header == NULL || process == NULL) {return -1;}
	
	/* ready state of process is set to 1, all other set to 0 */
	process->flags = set_flags(process->flags, 31);
	

	/* insert process into end of ready_queue */
	header->ready_queue = insert_last(header->ready_queue, process);
	
	return 0;
}

/* Move the process with matching pid from Ready to Stopped.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int schedule_suspend(schedule_header_t *header, pid_t pid) {

	/*error checking on input*/
	if(header == NULL){return -1;}

	/* finding the matching pid and save it to pid_node, delete from ready_queue */
	process_node_t *head = header->ready_queue->head;
	process_node_t *pid_node = find_remove_pid(&head, pid);
	if(pid_node != NULL){header->ready_queue->count -= 1;}

	/*setting suspended state bit to 1 and all other states to 0 */
	pid_node->flags = set_flags(pid_node->flags, 30);	

	/* inserting process into suspended queue */
	header->suspended_queue = insert_ascending(header->suspended_queue, pid_node);

	return 0;
}

/* Move the process with matching pid from Stopped to Ready.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int schedule_resume(schedule_header_t *header, pid_t pid) {

	/*checking if header input is valid*/
	if(header == NULL){return -1;}

	/* finding and removing matching pid in suspended queue */
	process_node_t *head = header->suspended_queue->head;
        process_node_t *pid_node = find_remove_pid(&head, pid);
        if(pid_node != NULL){header->suspended_queue->count -= 1;}

	/*setting ready state to 1 and other states to 0*/
	pid_node->flags = set_flags(pid_node->flags, 31);

	/* insert process into end of ready_queue */
	header->ready_queue = insert_last(header->ready_queue, pid_node);

	return 0;
}

/* Inserts the process node into the Defunct queue.
 * Follow the specification for this function.
 * Returns its exit code (from flags) on success or a -1 on any error.
 */
int schedule_exit(schedule_header_t *header, process_node_t *node, int exit_code) {

	if(node == NULL || header == NULL){return -1;}

	/*process to defunct state*/
	node->flags = set_flags(node->flags, 29);
	
	/* setting last 28 bits of exit_code to flags last 28 bits */
	int last28 = node->flags;
	int first3 = exit_code;   
	last28 = (last28 << 3);
	last28 = (last28 >> 3);
	first3 = (first3 >> 28);
	first3 = (first3 << 28);
	exit_code = last28 | first3;

	/* insert process into defunct queue in ascending pid order */
	header->defunct_queue = insert_ascending(header->defunct_queue, node);

	return exit_code;
}

/* Remove the process with matching pid from Ready or Suspended Queues and add the Exit Code to it.
 * Follow the specification for this function.
 * Returns its exit code (from flags) on success or a -1 on any error.
 */
int schedule_terminate(schedule_header_t *header, pid_t pid, int exit_code) {

	if(header == NULL){return -1;}

	/* finding matching pid in ready queue first */
	process_node_t *head = header->ready_queue->head;	
        process_node_t *pid_node = find_remove_pid(&head, pid);
        if(pid_node != NULL){header->ready_queue->count -= 1;}

	/* goes into suspened queue if not found in ready queue */
	else{
		head = header->suspended_queue->head;
		pid_node = find_remove_pid(&head, pid);
		/* did not find matching pid in neither queue, return -1 */
		if(pid_node == NULL){return -1;}
		else{header->suspended_queue->count -= 1;}
	}

	/* process to defunct state */
        pid_node->flags = set_flags(pid_node->flags, 29);

	/*setting flags bits used to exix_code to the value of exit_code passed it*/
	int last28 = pid_node->flags;
        int first3 = exit_code;
        last28 = (last28 << 3);
        last28 = (last28 >> 3);
        first3 = (first3 >> 28);
        first3 = (first3 << 28);

        exit_code = last28 | first3;

	/*inserting process into defunct queue in ascending pid order*/
	header->defunct_queue = insert_ascending(header->defunct_queue, pid_node);

	return exit_code;
}

/* Create a new process_node_t with the given information.
 * - Malloc and copy the command string, don't just assign it!
 * Follow the specification for this function.
 * Returns the process_node_t on success or a NULL on any error.
 */
process_node_t *schedule_new_process(char *command, pid_t pid, int sudo) {
	
	process_node_t *new_node = malloc(sizeof(process_node_t));
	if(new_node == NULL){return NULL;}

	new_node->pid = pid;
	int flags = 0;

	/*bits of flags into ready state*/
	flags = (1 << 31);

	/*initialize sudo flags*/
	if(sudo){flags = flags | (1 << 28);}	
	else{flags = flags & ~(1 << 28);}

	new_node->flags = flags;
	
	/*initizalize next and cmd*/	
	new_node->next = NULL;

	strncpy(new_node->cmd, command, (strlen(command)+1));
	
	return new_node;
}

/* Schedule the next process to run from Ready Queue.
 * Follow the specification for this function.
 * Returns the process selected or NULL if none available or on any errors.
 */
process_node_t *schedule_select(schedule_header_t *header) {
	
	/*checking if input is valid*/
	if(header == NULL){return NULL;}
	
	process_node_t *first_node = header->ready_queue->head;
	/*return the head*/
	header->ready_queue->head = header->ready_queue->head->next;
	header->ready_queue->count -= 1;

	return first_node;
}

/* Returns the number of items in a given queue_header_t
 * Follow the specification for this function.
 * Returns the number of processes in the list or -1 on any errors.
 */
int schedule_get_size(queue_header_t *ll) {
	
	if(ll == NULL){return -1;}
	return ll->count;
}

/* Frees all allocated memory in the schedule_header_tr */
void schedule_deallocate(schedule_header_t *header) {
	
	if(header == NULL){return;}
	
	free(header->ready_queue->head);
	free(header->ready_queue);
	free(header->suspended_queue->head);
	free(header->suspended_queue);
	free(header->defunct_queue->head);
	free(header->defunct_queue);
	free(header);
	return;
}

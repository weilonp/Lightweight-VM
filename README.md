# Lightweight-VM

***IMPORTANT: the key file vm_schedule.c is removed from the folder, only authorized accounts will be able to view vm_schedule.c

This is a lightweight process-level virtual machine (written in C) that allows Linux programs to run with custom execution commands. These machines are useful for testing complicated interactions between processes (programs being run) by manually scheduling them in certain orders, or allow the user to run processes with custom priority orderings.


To run the program:
type in ./vm in the command line

Built-In Commands to Control the CS Engine
• start - This will start the CS Engine running.
• stop - This will stop the CS Engine running. (You can start and stop it as much as you want!)
• status - This will print out a status message about the CS Engine settings and if it’s running.
• runtime X - This will change the runtime to a new usec value. (Default 250000 usec)
• delaytime X - This will change the delaytime to a new usec value. (Default 1000000 usec)

Example run:


<img width="497" alt="Screen Shot 2022-08-09 at 10 37 11 PM" src="https://user-images.githubusercontent.com/104396688/183798171-b97ca13d-a359-4ca3-92d7-453cf4c48e02.png">


Additional notes:
This project mainly uses linked-list to schedule the processes. The source code manually allocates memory at run time and frees all allocated memory when user quits the program. 
The VM scheduler uses an algorithm called Round Robin (RR) to select theprocess from the Ready Queue. Each process will then run for a small period of time
(quantum) and then get returned to the Ready Queue if not finished, or to the Defunct Queue if it finished in that time period.

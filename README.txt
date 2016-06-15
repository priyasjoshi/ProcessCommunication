Simple Bidding System
Features:
------------------------------------------------------------------------------------------
- This is a simple program implemented in C, where user will enter number of child processes to be created.
- The main program will act as a manager process and will send message to each child process created.
- The child process will receive the message and start generating the random number in sendbid() method.
- On generating the random number the number is sent to the parent by child using pipes.
- The parent will receive the random numbers and bid for the highest random number, the child process 
  with the highest random number is killed last.

IPC Mechanism used :
--------------------------------------------------------------------------------------------
- Pipes

Bugs:
--------------------------------------------------------------------------------------------

- Sometimes program execution results into Segmentation fault core dumped error.

Limitations:
---------------------------------------------------------------------------------------------

- Does not check for tie when bidding. 

How to Run : 
----------------------------------------------------------------------------------------------
- Compile the process.c file using gcc -o test process.c
- Run the program using ./test
- Enter the No of processes more than 3

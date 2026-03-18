# CMPE 382 – Homework 1

This project is for the CMPE 382 Operating Systems course.

## What this homework is about

In this assignment, we work with basic process management and communication in Linux.  
The program creates multiple processes and makes them communicate with each other.

## Topics covered

- fork() → creating child processes  
- pipe() → communication between processes  
- IPC (inter-process communication)

## Language

- C programming language

## Platform

- GNU/Linux

## Notes

The program creates two child processes.  
Each child works on a different file and sends its result to the parent process.  
The parent then shares the results back so that each child can determine the winner. 

## How to run the program

1. Open terminal and go to the project folder

2. Compile the code:
   gcc CMPE382-HW1_CK-12133216098.c -o hw1

3. Run the program:
   ./hw1

4. Enter a value for N when prompted

## Output

- The program creates two files: File1.txt and File2.txt  
- Each file contains N random numbers  
- Child processes calculate prime and abundant counts  
- Final results and the winner are printed on the terminal
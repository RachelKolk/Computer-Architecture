Files:
    FAQ - resource for questions and additional info
    LSA-Cheatsheet - a listing of operations written in hex
    LS8-Spec - specs for the whole project - extensive information about how things should work
    ls8 -
        - cpu.c - coding for all of the functions will go here
        - cpu.h - defines the cpu struct and the operations
        - ls8.c - holds the main and runs the cpu functions
        - makefile - holds termianl commands etc
        - readme - implementation instructions are in the readme
        - examples - files containing the instructions/programs for the ls8 written in hex

To be implemented: 
    - set up the struct in cpu.c
    - add function for ram-read
    - add function for ram-write
    - implement cpu-init
    - LOAD - LDI instruction
    - PRINT - PRN instruction
    - HALT - HLT instruction
    - fix the hardcoded cpu_load
    - get the cpu_load function coded so that it can run using an .ls8 file as an argument
    - implement the multiply instruction and run the mult8.ls8 file
    - implement system stack and run the stack.ls8 program
    - implement both CALL and RET instructions
    - implement subroutine calls abd run the call.ls8 program 

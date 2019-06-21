What are some examples of instructions handled by the CPU (as opposed to the ALU)?

    The CPU focuses on carrying out instructions of a computer program. It decodes the instructions
    to find out the operands and opcode. The alu handles all of the arithmetic and logic functions.

Convert the 8-bit hex number 0xB7 to binary.

    11 7 
    1011 0111

Convert the 8-bit binary number 0b10101011 to hex.

    AB

Describe the bitwise operations needed to turn off the 5th bit from the right of 0b01111100.

    XOR (^) it takes two numbers as operands and does XOR on every bit of the two numbers.
    The result of XOR is 1 if the two bits are different.

Why do people tend to gravitate towards base-10 number systems while computers do better with base-2?

    Because with base-2 number systems the computer only needs to read 1 or 0s to figure it out
    but with base-10 it would need to use 0-9 and therefore it would take more memory and processing time so base-2 is faster and cheaper and smaller. Base-10 is easier for humans
    to compute.

Create an AND mask to preserve the last 3 right-hand bits of any 8-bit number.

    x & 0b00000111

Describe the relative strengths and weaknesses RAM versus registers.

    Registers hold the operands or instructions that the CPU is currently processing.
    Where as, RAM holds the instructions and data the the currently executing program in CPU requires.
    Registers hold a small amount of data.
    But the memory(RAM) on a computer can be really quite large.
    CPU can operate on register contents at the rate of more than one operation per clock cycle.
    CPU accesses RAM at a slower rate than register. 

Describe the difference in what happens when a JUMP command is executed versus a CALL command.

    JUMP jumps to the address stored in the given register, and then sets the PC to the address
    stored in the given address.
    Where as, CALL pushes the address of the instruction directly after it onto the stack and
    then we jump to the location and do the function, but because we stored the next instruction's
    address we are able to go back to it after the call subroutine finishes executing. 
#include "cpu.h"
#include "stdlib.h"
#include "stdio.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

// RAM Read Function
unsigned char cpu_ram_read(struct cpu *cpu, int index) 
{
  return cpu->ram[index];
}

// RAM Write Function
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char value, int index)
 {
   return cpu->ram[index] = value;
 }


/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  
  // ir is the Instruction register (or the currently running instruction)
  unsigned char ir;
  // first operand declaration
  unsigned char operand_a;
  // second operand declaration
  unsigned char operand_b;
  // operand count declaration
  int operand_count;
  int reg_index;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    ir = cpu_ram_read(cpu, cpu->pc);
    // 2. Figure out how many operands this next instruction requires
    operand_count = ir >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    operand_a = cpu_ram_read(cpu, cpu->pc + 1);
    operand_b = cpu_ram_read(cpu, cpu->pc + 2);
    // 4. switch() over it to decide on a course of action.
    switch (ir) 
    {
    // 5. Do whatever the instruction should do according to the spec.

      case LDI:
        printf("Executing LDI\n");
        //printf("register a = %c\n", operand_a);
        // set register at first operand to the value of the second operand
        reg_index = operand_a & 0b00000111;
        cpu->registers[reg_index] = operand_b;        
        break;

      case PRN:
      printf("Executing PRN\n");
        // access register at first operand register
        reg_index = operand_a & 0b00000111;
        // then print the value
        printf("%d\n", cpu->registers[reg_index]);
        break;

      case HLT:
        printf("Executing HLT\n");
        running = 0;
        break;

      default:
        printf("Unrecognized instruction\n");
        exit(1);
        break;
    }
    
    // 6. Move the PC to the next instruction.
    cpu->pc += operand_count + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // set program counter (index of current instruction) to 0
  cpu->pc = 0;
}

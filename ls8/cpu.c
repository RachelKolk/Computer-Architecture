#include "cpu.h"

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
  unsigned char register_a;
  // second operand declaration
  unsigned char register_b;
  // operand count declaration
  int operand_count;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    ir = cpu_ram_read(cpu, cpu->pc);
    // 2. Figure out how many operands this next instruction requires
    operand_count = ir >> DATA_LEN;
    // 3. Get the appropriate value(s) of the operands following this instruction
    register_a = cpu_ram_read(cpu, cpu->pc + 1);
    register_b = cpu_ram_read(cpu, cpu->pc + 2);
    // 4. switch() over it to decide on a course of action.
    switch (ir) 
    {
    // 5. Do whatever the instruction should do according to the spec.
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

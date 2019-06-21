#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6
#define SP 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp;
  char line[1024];
  int address = 0;
  // opens the file
  fp = fopen(filename, "r");

  // checks to make sure the file exists
  if (fp == NULL)
  {
    fprintf(stderr, "Error opening file %s\n", filename);
    exit(1);
  }

    while (fgets(line, 1024, fp) != NULL)
    {
      char *endptr;
      // parse line for number, copying first non number char to endptr
      unsigned char value = strtoul(line, &endptr, 2);
      // check if no numbers were found
      if (endptr == line)
      {
        continue;
      }
      // store the value into memory
      cpu->ram[address++] = value;
    }
  // close at EOL
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  int reg_index;
  unsigned char value;

  switch (op) {
    case ALU_MUL:
      //printf("Executing MUL\n");
      // find index of register b
      reg_index = regB & 0b00000111;
      // set it to a temp var value
      value = cpu->registers[reg_index];
      // then find the index of register a
      reg_index = regA & 0b00000111;
      // update the value temp var to (value * value at register a)
      value = value * cpu->registers[reg_index];
      // store the resulting product in register a
      cpu->registers[reg_index] = value;      
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      //printf("Executing ADD\n");
      // set value of operand_b to temp var
      value = cpu->registers[regB];
      // add the value of operand_a to the var
      value = value + cpu->registers[regA];
      // store the result in register a
      cpu->registers[regA] = value;
      break;

    case ALU_CMP:
      //`FL` bits: `00000LGE`
      //printf("Executing CMP\n");
      // compare the values in two registers
      if (cpu->registers[regA] == cpu->registers[regB])
      {
        // set equal flag to 1
        cpu->flag ^= 0b00000001;
      } else if (cpu->registers[regA] > cpu->registers[regB])
      {
        // set greater than flag to 1
        cpu->flag ^= 0b00000010;
      } else 
      {
        // set less than flag to 1
        cpu->flag ^= 0b00000100;
      }
      break;
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
 * Handler Functions for Instructions
 */
void ldi_handler(struct cpu *cpu, unsigned char operand_a, unsigned char operand_b)
{
  //printf("Executing LDI\n");
  // set register at first operand to the value of the second operand
  int reg_index = operand_a & 0b00000111;
  cpu->registers[reg_index] = operand_b;  
}

void prn_handler(struct cpu *cpu, unsigned char operand_a)
{
  //printf("Executing PRN\n");
  // access register at first operand register
  int reg_index = operand_a & 0b00000111;
  // then print the value
  printf("%d\n", cpu->registers[reg_index]);   
}

void push_handler (struct cpu *cpu, unsigned char operand_a)
{
  //printf("Executing PUSH\n");
  // decrement the stack pointer
  cpu->registers[SP]--;
  // get the value from the register index of operand_a
  int value = cpu->registers[operand_a];
  // push the value onto the stack using the write function
  cpu_ram_write(cpu, value, cpu->registers[SP]);
}

void pop_handler (struct cpu *cpu, unsigned char operand_a)
{
  //printf("Executing POP\n");
  // read the stack pointer registers
  int value = cpu_ram_read(cpu, cpu->registers[SP]);
  // store the value from the register index of operand_a
  cpu->registers[operand_a] = value;
  //increment the stack pointer
  cpu->registers[SP]++; 
}
   
void call_handler (struct cpu *cpu, unsigned char operand_a)
{
  //printf("Executing CALL\n");
  // decrement the stack pointer
  cpu->registers[SP]--;
  // save the instruction after CALL to the stack using the write function
  cpu_ram_write(cpu, cpu->pc + 2, cpu->registers[SP]);
  // set the pc to the value of register index operand_a
  cpu->pc = cpu->registers[operand_a];
}

void ret_handler (struct cpu *cpu)
{
  //printf("Executing RET\n");
  // pop the value form the top of the stack and store it in the pc
  cpu->pc = cpu_ram_read(cpu, cpu->registers[SP]);
  // increment the stack pointer
  cpu->registers[SP]++;
}

void jmp_handler (struct cpu *cpu, unsigned char operand_a)
{
  //printf("Executing JMP\n");
  // jump to the address stored in the given register by setting the pc value
  cpu->pc = cpu->registers[operand_a];
}

void jeq_handler (struct cpu *cpu, unsigned char operand_a, int operand_count)
{
  //printf("Executing JEQ\n");
  // check to see if the equal flag is true (1)
  if (cpu->flag & 0b00000001)
  {
    // if so, jump to the address stored at the given register by setting the pc value
    cpu->pc = cpu->registers[operand_a];
  } else
  {
    // manually set the pc value
    cpu->pc += operand_count + 1;
  }
}

void jne_handler (struct cpu *cpu, unsigned char operand_a, int operand_count)
{
  //printf("Executing JNE\n");
  // if the equal flag is false (0)
  if (!(cpu->flag & 0b00000001))
  {
    // jump to the address stored at the given register by setting the pc value
    cpu->pc = cpu->registers[operand_a];
  } else
  {
    // manually set the pc value
    cpu->pc += operand_count + 1;
  }
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
        ldi_handler(cpu, operand_a, operand_b);        
        break;

      case PRN:
        prn_handler(cpu, operand_a);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operand_a, operand_b);
        break;

      case PUSH:
        push_handler(cpu, operand_a);
        break;

      case POP:
        pop_handler(cpu, operand_a);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operand_a, operand_b);
        break;

      case CALL:
        call_handler(cpu, operand_a);
        break;

      case RET:
        ret_handler(cpu);
        break;

      case CMP:
        alu(cpu, ALU_CMP, operand_a, operand_b);
        break;

      case JMP:
        jmp_handler(cpu, operand_a);
        break;

      case JEQ:
        jeq_handler(cpu, operand_a, operand_count);
        // reset flag to 0
        cpu->flag = 0;
        break;

      case JNE:
        jne_handler(cpu, operand_a, operand_count);
        // reset flag to 0
        cpu->flag = 0;
        break;

      case HLT:
        //printf("Executing HLT\n");
        running = 0;
        break;

      default:
        printf("Unrecognized instruction\n");
        exit(1);
        break;
    }
    
    // check to see if ir sets the pc directly &
    // 6. Move the PC to the next instruction depending
    if (!(ir & SETS_PC))
    {
      cpu->pc += operand_count + 1;
    }
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
  // set all register and ram bytes to 0 bits using memset
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  // set the stack pointer to F4
  cpu->registers[SP] = 244;
  // set flag to 0
  cpu->flag = 0;
}

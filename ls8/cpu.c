#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"
#include <string.h>
#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char val)
{
  cpu->ram[index] = val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *fn_arg)
{
  // TODO: Replace this with something less hard-coded
  FILE *fp;
  char line[1024];
  int address = 0;

  fp = fopen(fn_arg, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "comp: error opening file\n");
    exit(2);
  }
  while (fgets(line, 1024, fp) != NULL)
  {
    char *endptr;

    unsigned int val = strtoul(line, &endptr, 2);

    if (endptr == line)
    {
      //printf("Found no digits\n");
      continue;
    }
    //printf("%u\n", val);
    cpu->ram[address] = val;
    address++;
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char operandA;
  unsigned char operandB;

  while (running)
  {
    unsigned int ops;
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char ir = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    ops = ir >> 6; // number of operations are stored in bits #6-7
    // 3. Get the appropriate value(s) of the operands following this instruction
    operandA = cpu_ram_read(cpu, cpu->PC + 1);
    operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 4. switch() over it to decide on a course of action.
    switch (ir)
    {
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    case LDI: // 2 operands
      // set the value of a register to an integer
      cpu->registers[operandA] = operandB;
      cpu->PC += 1 + ops;
      break;
    case PRN: // PRN, 1 operands
      // Print to the console the decimal integer value stored in the given register
      printf("%d\n", cpu->registers[operandA]);
      cpu->PC += 1 + ops;
      break;
    case HLT: // HLT, no operands
      running = 0;
      cpu->PC += 1 + ops;
      break;
    default: // instruction not found
      printf("Unknown instruction at PC: %d", cpu->PC);
      exit(1);
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu = malloc(sizeof(cpu));
  cpu->PC = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

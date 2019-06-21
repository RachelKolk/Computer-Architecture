#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  // checks for exactly 2 arguments
  if (argc != 2)
  {
    fprintf(stderr, "usage: comp filename\n");
    return 1;
  }

  cpu_init(&cpu);
  cpu_load(&cpu, argv[1]);
  cpu_run(&cpu);

  return 0;
}
#include "airbag.h"
#include <stdio.h>

void hej()
{
  int* ptr = 0;
  *ptr = 0;
}


void try_to_crash()
{
  hej();
}

int main()
{
  FILE* file = fopen("bin/airbag.out", "w");
  int fd = fileno(file);

  airbag_init(fd);

  try_to_crash();

  int y = 0;
  int x = 3 / y;
  fprintf(file, "x = %d\n", x);

  return 0;
}
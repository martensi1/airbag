#include "airbag.h"
#include <stdio.h>


int main()
{
  FILE* file = fopen("log.txt", "w");
  int fd = fileno(file);

  airbag_init(fd);

  int* ptr = 0;
  *ptr = 0;

  int y = 0;
  int x = 3 / y;
  fprintf(file, "x = %d\n", x);

  return 0;
}
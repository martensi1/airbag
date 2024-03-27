#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../airbag.h"


const char* get_option(int argc, char* argv[], const char* option)
{
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], option) == 0 && i + 1 < argc)
    {
      return argv[i + 1];
    }
  }

  return NULL;
}


static int be_bad(const char* crash_type, const char* output_file)
{
    FILE* file = fopen(output_file, "w");
    int fd = fileno(file);

    airbag_init(fd);

    if (strcmp(crash_type, "abort") == 0)
    {
      abort();
    }
    else if (strcmp(crash_type, "access-violation") == 0)
    {
      int* ptr = 0;
      *ptr = 0;
    }
    else if (strcmp(crash_type, "divide-by-zero") == 0)
    {
      int y = 0;
      int x = 3 / y;
    }
    else
    {
      printf("Unknown crash type: %s\n", crash_type);
      return EXIT_FAILURE;
    }

    // This point should never be reached
    airbag_cleanup();
    fclose(file);
}


int main(int argc, char* argv[])
{
    const char* chrash_type = get_option(argc, argv, "-t");
    const char* output_file = get_option(argc, argv, "-o");

    if (chrash_type == NULL || output_file == NULL)
    {
      printf("Usage: %s -t <type> -o <output file>\n", argv[0]);
      return 1;
    }

    be_bad(chrash_type, output_file);
}


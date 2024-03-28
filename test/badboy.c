////////////////////////////////////////////////////////////
//
// Airbag Crash Handler
// Copyright (C) 2024 Simon Alm-Mårtensson
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#include "../airbag.h"


//---------------------------------------------------------
// Crash functions
//---------------------------------------------------------
static void abort_program();
static void access_violation();
static void divide_by_zero();
static void illegal_instruction();
static void bus_error();
static void invalid_syscall();


static int be_bad(const char* crash_type)
{
    if (strcmp(crash_type, "abort") == 0)
    {
      abort_program();
    }
    else if (strcmp(crash_type, "access-violation") == 0)
    {
      access_violation();
    }
    else if (strcmp(crash_type, "divide-by-zero") == 0)
    {
      divide_by_zero();
    }
    else if (strcmp(crash_type, "illegal-instruction") == 0)
    {
      illegal_instruction();
    }
    else if (strcmp(crash_type, "bus-error") == 0)
    {
      bus_error();
    }
    else if (strcmp(crash_type, "system-call-bad-argument") == 0)
    {
      invalid_syscall();
    }
    else
    {
      printf("Unknown crash type: %s\n", crash_type);
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


static void abort_program()
{
    abort();
}

static void access_violation()
{
    int* ptr = 0;
    *ptr = 0;
}

static void divide_by_zero()
{
    int y = 0;
    int x = 3 / y;
}

static void illegal_instruction()
{
#if defined(__i386__) || defined(__x86_64__)
    asm("lgdt 0");
#else
    #error "No illegal instruction for arhitecture"
#endif
}

static void bus_error()
{
    FILE *f = tmpfile();
    int *m = mmap(0, 4, PROT_WRITE, MAP_PRIVATE, fileno(f), 0);
    *m = 0;
}

static void invalid_syscall()
{
  
  syscall(2342347234);
}


//---------------------------------------------------------
// Main
//---------------------------------------------------------
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

int main(int argc, char* argv[])
{
    const char* chrash_type = get_option(argc, argv, "-t");
    const char* output_file = get_option(argc, argv, "-o");

    if (chrash_type == NULL || output_file == NULL)
    {
      printf("Usage: %s -t <type> -o <output file>\n", argv[0]);
      return 1;
    }

    FILE* file = fopen(output_file, "w");
    int fd = fileno(file);
    airbag_init(fd);

    int result = be_bad(chrash_type);

    // This point should never be reached
    airbag_cleanup();
    fclose(file);

    return result;
}


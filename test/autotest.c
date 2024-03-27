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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define HGRN "\e[0;92m"
#define HRED "\e[0;91m"
#define COLOR_RESET "\e[0m"


static const char* all_tests[] = {
  "abort",
  "access-violation",
  "divide-by-zero",
  "sigbus"
};


//---------------------------------------------------------
// File I/O
//---------------------------------------------------------
static int get_file_size(FILE* file)
{
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

static void remove_out_files()
{
  DIR* dir = opendir("./");

  if (dir == NULL)
  {
    return;
  }

  struct dirent* entry;

  while ((entry = readdir(dir)) != NULL)
  {
    // remove all .out files
    if (strstr(entry->d_name, ".out") == NULL)
    {
      continue;
    }

    remove(entry->d_name);
  }
}



static int exec_badboy(const char* crash_type, const char* filename)
{
  char command[256];
  strcpy(command, "bin/badboy -t ");
  strcat(command, crash_type);
  strcat(command, " -o ");
  strcat(command, filename);
  strcat(command, " > /dev/null 2>&1");

  int ec = system(command);
  return ec;
}

static bool run_test(const char* test)
{
  char filename[256];
  strcpy(filename, test);
  strcat(filename, ".out");

  int ec = exec_badboy(test, filename);

  FILE* file = fopen(filename, "r");
  
  bool dump_written = (file != NULL) && (get_file_size(file) > 0);
  bool test_passed = dump_written;

  const char* result_text = dump_written ? "OK" : "FAIL";
  const char* result_color = dump_written ? HGRN : HRED;

  printf("[%s%s%s]: %s\n", result_color, result_text, COLOR_RESET, test);
  fflush(stdout);
}

int main()
{
  remove_out_files();

  for (int i = 0; i < sizeof(all_tests) / sizeof(all_tests[0]); i++)
  {
    run_test(all_tests[i]);
  }

  remove_out_files();

  return EXIT_SUCCESS;
}

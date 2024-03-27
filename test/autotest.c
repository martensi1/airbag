#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define HGRN "\e[0;92m"
#define HRED "\e[0;91m"
#define COLOR_RESET "\e[0m"


static const char* _tests[] = {
  "abort",
  "access-violation",
  "divide-by-zero"
};


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

int get_file_size(FILE* file)
{
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
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
  for (int i = 0; i < sizeof(_tests) / sizeof(_tests[0]); i++)
  {
    run_test(_tests[i]);
  }

  return 0;
}

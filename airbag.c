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
#include "airbag.h"

//#define _POSIX_C_SOURCE 200809L
//#define _POSIX_C_SOURCE 200809L


//---------------------------------------------------------
// Headers
//---------------------------------------------------------
#include <signal.h>   // sigaction, siginfo_t, sig_atomic_t
#include <unistd.h>   // write, getpid
#include <string.h>   // strlen
#include <stdbool.h>  // bool
#include <execinfo.h> // backtrace, backtrace_symbols_fd


//---------------------------------------------------------
// Platform and compiler specific checks
//---------------------------------------------------------
#if !defined (__linux__) && !defined (__unix__)
  #error "Only POSIX compliant systems are supported"
#endif

#if defined(__GNUC__)
  #define FORCE_INLINE inline __attribute__((always_inline))
#else
  #define FORCE_INLINE inline
#endif

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------
// Static variables
//---------------------------------------------------------
static int airbag_fd = -1;

static const char* signal_names[] = {
  NULL,
  "SIGHUP",    // 0 - Hangup
  "SIGINT",    // 1 - Interrupt
  "SIGQUIT",   // 2 - Quit
  "SIGILL",    // 3 - Illegal instruction
  "SIGTRAP",   // 4 - Trace/breakpoint trap
  "SIGABRT",   // 6 - Aborted
  "SIGBUS",    // 7 - Bus error
  "SIGFPE",    // 8 - Floating point exception
  "SIGKILL",   // 9 - Killed
  "SIGUSR1",   // 10 - User defined signal 1
  "SIGSEGV",   // 11 - Segmentation fault
  "SIGUSR2",   // 12 - User defined signal 2
  "SIGPIPE",   // 13 - Broken pipe
  "SIGALRM",   // 14 - Alarm clock
  "SIGTERM",   // 15 - Terminated
  "SIGSTKFLT", // 16 - Stack fault
  "SIGCHLD",   // 17 - Child exited
  "SIGCONT",   // 18 - Continued
  "SIGSTOP",   // 19 - Stopped
  "SIGTSTP",   // 20 - Stopped (tty input)
  "SIGTTIN",   // 21 - Stopped (tty background read)
  "SIGTTOU",   // 22 - Stopped (tty background write)
  "SIGURG",    // 23 - Urgent I/O condition
  "SIGXCPU",   // 24 - CPU time limit exceeded
  "SIGXFSZ",   // 25 - File size limit exceeded
  "SIGVTALRM", // 26 - Virtual timer expired
  "SIGPROF",   // 27 - Profiling timer expired
  "SIGWINCH",  // 28 - Window size change
  "SIGIO",     // 29 - I/O now possible
  "SIGPWR",    // 30 - Power failure
  "SIGSYS"     // 31 - Bad system call
};


//---------------------------------------------------------
// Static functions
//---------------------------------------------------------
static char* itoa(int value, char * buffer, int base)
{
    char* ptr = buffer;
    bool negative = false;

    if (value == 0) {
        *ptr++ = '0';
        *ptr++ = '\0';

        return buffer;
    }
 
    // Negative numbers only handled for base 10
    if (value < 0 && base == 10) {
        negative = true;
        value = -value;
    }
 
    // Process individual digits (from right to left)
    // Use remainder to get the digits for each position
    while (value != 0) {
        int remainder = value % base;

        char c = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        *ptr++ = c;

        value = value / base;
    }
 
    if (negative)
        *ptr++ = '-';
 
    *ptr = '\0';
 
    // Done!
    // Reverse the string to get the correct order
    ptr--;
    char* start = buffer;

    while (start < ptr) {
        char temp = *start;
        *start++ = *ptr;
        *ptr-- = temp;
    }

    return buffer;
}

static inline void dwrite(const char* str)
{
  write(airbag_fd, str, strlen(str));
}

static void dwriteint(int value, int base)
{
  char buffer[32];
  itoa(value, buffer, base);
  dwrite(buffer);
}

static const char* get_signal_name(int signum)
{
  if (signum > 0 && signum <= sizeof(signal_names) / sizeof(signal_names[0])) {
    return signal_names[signum];
  }

  return "UNKNOWN";
}

static void handle_crash(siginfo_t* info)
{
  dwrite("===Airbag===\n");

  dwrite("Signal: ");
  dwrite(get_signal_name(info->si_signo));
  dwrite("\n");

  dwrite("PID: ");
  dwriteint(getpid(), 10);
  dwrite("\n");

  dwrite("UID: ");
  dwriteint(getuid(), 10);
  dwrite("\n\n");

  dwrite("Backtrace:\n");

  void* array[10];
  int size;

  size = backtrace(array, 10);
  backtrace_symbols_fd(array, size, airbag_fd);
}

static void handle_signal(const int signum, siginfo_t* info, void* context)
{
  static volatile sig_atomic_t first_call = 1;

  if (first_call) {
    first_call = 0;
    handle_crash(info);
  }

  // Re-raise the signal to invoke the default signal handler
  signal(signum, SIG_DFL);
  raise(signum);
}


//---------------------------------------------------------
// Public functions
//---------------------------------------------------------
void airbag_init(int fd)
{
  airbag_fd = fd;

  // Dummy backtrace to initialize the backtrace system
  // This is necessary to make the backtrace function async-signal-safe,
  // see more in man page for backtrace
  void* dummy = NULL;
  backtrace(&dummy, 1);

  // Set up signal handlers with extended signal handling (siginfo_t and context)
  // and dedicated/alternate signal stack
  int flags = 0;
  flags |= SA_ONSTACK;
  flags |= SA_SIGINFO;

  struct sigaction action = (struct sigaction) {
    .sa_flags = flags,
    .sa_sigaction = handle_signal
    // Do not assign .sa_handler, it will override
    // .sa_sigaction if they are implemented with a union
  };

  sigemptyset(&action.sa_mask);

  sigaction(SIGBUS, &action, NULL);
  sigaction(SIGFPE, &action, NULL);
  sigaction(SIGILL, &action, NULL);
  sigaction(SIGSEGV, &action, NULL);
  sigaction(SIGSYS, &action, NULL);
  sigaction(SIGTRAP, &action, NULL);
  sigaction(SIGXCPU, &action, NULL);
  sigaction(SIGXFSZ, &action, NULL);
}

void airbag_cleanup()
{
  airbag_fd = -1;

  // Reset signal handlers to default
  signal(SIGBUS, SIG_DFL);
  signal(SIGFPE, SIG_DFL);
  signal(SIGILL, SIG_DFL);
  signal(SIGSEGV, SIG_DFL);
  signal(SIGSYS, SIG_DFL);
  signal(SIGTRAP, SIG_DFL);
  signal(SIGXCPU, SIG_DFL);
  signal(SIGXFSZ, SIG_DFL);
}

#ifdef __cplusplus
}
#endif
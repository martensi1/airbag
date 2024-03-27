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

#include <signal.h>
#include <unistd.h>
#include <string.h>


static int signal_fd = -1;


#ifdef __cplusplus
extern "C" {
#endif


static inline void swrite(const char* str)
{
  write(signal_fd, str, strlen(str));
}

static void handle_crash(siginfo_t* info)
{
  swrite("Crash detected\n");
  //fsync(signal_fd);
}

static void handle_signal(const int signum, siginfo_t* info, void* context)
{
  static volatile sig_atomic_t first_call = 1;

  if (first_call) {
    first_call = 0;
    handle_crash(info);
  }

  // Re-raise the signal to invoke the default signal handler

  raise(signum);
}


void airbag_init(int fd)
{
  signal_fd = fd;

  int flags = 0;
  flags |= SA_ONSTACK; // Use dedicated/alternate signal stack
  flags |= SA_SIGINFO; // Use extended signal handling (siginfo_t and context)

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

}

#ifdef __cplusplus
}
#endif
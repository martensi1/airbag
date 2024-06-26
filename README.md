# Airbag

Simple drop-in crash handler written in C for POSIX based systems. It allows for capturing crash signals such as segmentation faults, aborts, and other fatal signals, providing a mechanism to log diagnostic information before terminating the program.

## Usage

Add the `airbag.c` and `airbag.h` files to your project. Use the `airbag_init` function to install the crash handler and the `airbag_cleanup` function to remove it.

## Example

The following example demonstrates how to use the airbag library to capture crashes and log diagnostic information to a file.

```c
#include "airbag.h"
#include <stdio.h>

int main() {
  // Install the crash handler
  FILE* file = fopen("airbag.out", "w");
  int fd = fileno(file);

  airbag_init(fd);

  // Your program logic goes here
  // ...

  // Removes the crash handler
  airbag_cleanup();
  flose(file);
}
```

## Limitations

The printed backtrace will not always be reliable/complete if the code has been optimized during compilation. For example:

* Inlined functions will not appear in the backtrace (does not have stack frames)
* Omission of frame pointers may cause the backtrace to be incomplete (which is common in optimized builds)
* Tail-call optimization may cause the backtrace to be incomplete

## Resources

* https://www.man7.org/linux/man-pages/man7/signal.7.html
* https://man7.org/linux/man-pages/man2/sigaction.2.html
* https://man7.org/linux/man-pages/man7/signal-safety.7.html
* https://man7.org/linux/man-pages/man3/backtrace.3.html
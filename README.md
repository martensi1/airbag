# Airbag

Simple drop-in crash handler written in C for POSIX based systems. It allows for capturing crash signals such as segmentation faults, aborts, and other fatal signals, providing a mechanism to log diagnostic information before terminating the program.

## Usage

Add the `airbag.c` and `airbag.h` files to your project.

```c
#include "airbag.h"

int main() {
  // Install the crash handler
  airbag_init();

  // Your program logic goes here
  // ...

  // Removes the crash handler
  airbag_cleanup();
}
```

## Resources

* https://www.man7.org/linux/man-pages/man7/signal.7.html
* https://man7.org/linux/man-pages/man2/sigaction.2.html
* https://man7.org/linux/man-pages/man7/signal-safety.7.html
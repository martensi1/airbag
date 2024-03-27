

// Disable stack overflow warning
#pragma warning (disable: 4717)

// Disable buffer overrun warning
#pragma warning (disable: 4789)


typedef enum crash_type {
  CRASH_TYPE_ABORT,
  CRASH_TYPE_ACCESS_VIOLATION,
  CRASH_TYPE_STACK_OVERFLOW,
  CRASH_TYPE_DIVIDE_BY_ZERO,
} crash_type_t;

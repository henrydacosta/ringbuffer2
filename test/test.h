#ifndef _TEST_H
#define _TEST_H

#include <stddef.h>
#include <stdio.h>

extern size_t tests_run;
extern size_t tests_succeeded;

#define TEST(result)                                                           \
  do {                                                                         \
    ++tests_run;                                                               \
    if (result) {                                                              \
      ++tests_succeeded;                                                       \
    } else {                                                                   \
      printf("%s: %d: FAIL\n", __FILE__, __LINE__);                            \
    }                                                                          \
  } while (0)

#endif // _TEST_H

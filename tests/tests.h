#ifndef TESTS_H
#define TESTS_H

#define PASS 0
#define FAIL 1

#define ASSERT(expression, ...) \
  if (!(expression)) {          \
    printf(__VA_ARGS__);        \
    return FAIL;                \
  }

void run_test(const char* test_name, int (*test_fn)());

// Tests
int test_keyscan();
int test_keymap();

#endif  // TESTS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "tests.h"

static int passed = 0;
static int failed = 0;

const char* TEST_NAMES[] = {"keymap.c", "keyscan.c"};
const int (*TESTS[])() = {test_keymap, test_keyscan};
const uint8_t TESTS_LEN = 2;

static const char* ansi_bold = "\033[1m";
static const char* ansi_bold_red = "\033[1;31m";
static const char* ansi_red = "\033[31m";
static const char* ansi_bold_green = "\033[1;32m";
static const char* ansi_green = "\033[32m";
static const char* ansi_reset = "\033[0m";

void run_test(const char* test_name, int (*test_fn)()) {
  printf("\t%s[RUN]  %s%s\n", ansi_bold, test_name, ansi_reset);
  int result = test_fn();

  if (result == PASS) {
    printf("\t%s[PASS] %s%s\n", ansi_bold_green, test_name, ansi_reset);
    passed++;
  } else {
    printf("\t%s[FAIL] %s%s\n", ansi_bold_red, test_name, ansi_reset);
    failed++;
  }
}

int main() {
  printf("Running tests:\n\n");

  for (int i = 0; i < TESTS_LEN; i++) {
    run_test(TEST_NAMES[i], TESTS[i]);
  }

  if (failed == 0) {
    printf("\n%s%d of %d tests passed (%d failed)%s\n",
           ansi_bold_green,
           passed,
           passed + failed,
           failed,
           ansi_reset);
  } else {
    printf("\n%s%d of %d tests passed (%d failed)%s\n",
           ansi_bold_red,
           passed,
           passed + failed,
           failed,
           ansi_reset);
  }

  return failed != 0;
}

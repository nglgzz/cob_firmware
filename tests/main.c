#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "tests.h"

static int passed = 0;
static int failed = 0;

const char* test_names[] = {"keymap"};
const int (*tests[])() = {test_keymap};
const uint8_t tests_len = 1;

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

  for (int i = 0; i < tests_len; i++) {
    run_test(test_names[i], tests[i]);
  }

  printf("\n%d of %d tests passed (%d failed)\n", passed, passed + failed, failed);
  return failed ? 1 : 0;
}

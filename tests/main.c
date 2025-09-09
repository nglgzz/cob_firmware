#include <stdbool.h>
#include <stdio.h>

#include "tests.h"

static int passed = 0;
static int failed = 0;

static const char* ansi_bold = "\033[1m";
static const char* ansi_bold_red = "\033[1;31m";
static const char* ansi_red = "\033[31m";
static const char* ansi_bold_green = "\033[1;32m";
static const char* ansi_green = "\033[32m";
static const char* ansi_reset = "\033[0m";

void process_result(const char* test_name, bool result) {
  if (result) {
    printf("\t%s[PASS] -- %s%s\n", ansi_bold_green, test_name, ansi_reset);
    passed++;
  } else {
    printf("%s[FAIL] -- %s%s\n", ansi_bold_red, test_name, ansi_reset);
    failed++;
  }
}

int main() {
  bool result;

  printf("Running tests:\n\n");

  result = test_keymap();
  process_result("keymap", result);

  printf("\n%d of %d tests passed (%d failed)\n", passed, passed + failed, failed);
  return failed ? 1 : 0;
}

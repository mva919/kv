#include "parser.h"
#include <assert.h>
#include <string.h>

void test_parse_key_command() {
  // testing proper get command
  struct kv_command command;
  assert(parse_key_command("g,foo\0", &command) == 0);
  assert(command.command == get);
  assert(strcmp(command.key, "foo") == 0);
}

int main() { test_parse_key_command(); }

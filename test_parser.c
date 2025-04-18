#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_parse_key_command() {
  struct kv_command command;
  assert(parse_key_command("10", &command) == 0);
  assert(strcmp(command.key, "10") == 0);
  memset(&command, 0, sizeof command);
  assert(parse_key_command(",", &command) == -1);
  assert(command.key == 0);
}

void test_parse_key_val_command() {
  struct kv_command command;
  assert(parse_key_val_command("10,foo", &command) == 0);
  assert(strcmp(command.key, "10") == 0);
  assert(strcmp(command.value, "foo") == 0);
  memset(&command, 0, sizeof command);
  assert(parse_key_val_command("10,foo,", &command) == -1);
  assert(command.key == 0);
  assert(command.value == 0);
}

void test_parse_arguments() {
  int argc = 6;
  char *argv[] = {"p,10,foo", "g,10", "d,10", "a", "c"};
  struct kv_command *command_list[argc - 1];

  // assert(parse_arguments(argc, argv, command_list) == 5);
}

int main() {
  test_parse_key_command();
  test_parse_key_val_command();
  test_parse_arguments();

  return 0;
}

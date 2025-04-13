#include "parser.h"
#include <stdio.h>

int main(int argc, char **argv) {
  struct kv_command *command_list[argc - 1];
  int comman_list_len = parse_arguments(argc, argv, command_list);

  for (int i = 0; i < comman_list_len; i++) {
    printf("%d\n", command_list[i]->command);
  }

  return 0;
}

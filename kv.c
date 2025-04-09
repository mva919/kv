#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAX_ARG_LEN 40

typedef enum { put = 0, get = 1, delete = 2, clear = 3, all = 4 } command_type;

struct kv_command {
  command_type command;
  char *key;
  char *value;
};

int main(int argc, char **argv) {
  struct kv_command *command_list[argc - 1];

  for (int i = 1; i < argc; i++) {
    char command = argv[i][0];
    struct kv_command *curr_command = malloc(sizeof(struct kv_command));

    switch (command) {
    case 'p':
      curr_command->command = put;
      break;
    case 'g':
      curr_command->command = get;
      break;
    case 'd':
      curr_command->command = delete;
      break;
    case 'c':
      curr_command->command = clear;
      break;
    case 'a':
      curr_command->command = all;
      break;
    default:
      fprintf(stderr, "Error: invalid operation %d:'%s'.\n", i, argv[i]);
      free(curr_command);
      continue;
    }

    if (argv[i][1] != ',') {
      fprintf(stderr, "Error: invalid query syntax %d:'%s'.\n", i, argv[i]);
      free(curr_command);
      continue;
    }

    u_int8_t ptr = 2;
    while (argv[i][ptr] != '\0' && argv[i][ptr] != ',') {

      if (ptr == MAX_ARG_LEN) {
        fprintf(stderr,
                "Error: query %d value string is longer than 40 characters.\n",
                i);
        free(curr_command);
        continue;
      }
      ptr++;
    }

    if (argv[i][ptr] == ',') {
      if (curr_command->command == put) {
        argv[i][ptr] = '\0';
      }
    }

    u_int8_t str_len = ptr - 1;

    curr_command->key = malloc(str_len);
    if (curr_command->key == NULL) {
      fprintf(stderr,
              "Error: something went wrong while processing command.\n");
      free(curr_command);
      continue;
    }

    strncpy(curr_command->key, argv[i] + 2, str_len);

    if (curr_command->command == put) {

      if (argv[i][+ptr] != ',') {
        printf("%c\n", argv[i][ptr]);
        fprintf(stderr, "Error: invalid put query syntax '%s'.\n", argv[i]);
        continue;
      }
      u_int8_t initial_ptr_idx = ptr;
      while (argv[i][ptr] != '\0') {
        if (argv[i][ptr] == ',') {
          fprintf(stderr, "Error: invalid use of comma charcter %d: '%s'.\n", i,
                  argv[i]);
          free(curr_command);
          continue;
        }
        if (ptr == MAX_ARG_LEN) {
          fprintf(stderr,
                  "Error: value string is longer than 40 characters.\n");
          continue;
        }
        ptr++;
      }
      str_len = ptr - initial_ptr_idx;
      strncpy(curr_command->value, argv[i] + initial_ptr_idx, str_len);
      printf("%s\n", curr_command->value);
    }

    printf("%s\n", curr_command->key);
    command_list[i - 1] = curr_command;
  }

  // for (int i = 0; i < argc - 1; i++) {
  //   printf("%d, %s\n", command_list[i]->command, command_list[i]->key);
  // }

  return 0;
}

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_u_int8_t.h>
#include <sys/types.h>

#define MAX_ARG_LEN 40

typedef enum { put = 0, get = 1, delete = 2, clear = 3, all = 4 } command_type;

struct kv_command {
  command_type command;
  char *key;
  char *value;
};

int parse_key_command(char *str, struct kv_command *command) {
  u_int8_t i;
  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] == ',') {
      return -1;
    }
  }
  if (i == 0) {
    return -1;
  }

  command->key = malloc(i + 1);
  if (command->key == NULL) {
    return -1;
  }
  strncpy(command->key, str, i + 1);

  return 0;
}

int parse_key_val_command(char *str, struct kv_command *command) {
  u_int8_t comma_i, i;
  bool seen_comma = false;
  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] == ',') {
      if (seen_comma) {
        return -1;
      }
      seen_comma = true;
      comma_i = i;
    }
  }

  if (!seen_comma || comma_i + 1 == i) {
    return -1;
  }

  command->key = malloc(comma_i + 1);
  if (command->key == NULL) {
    return -1;
  }
  strncpy(command->key, str, comma_i);

  command->value = malloc(i - comma_i + 1);
  if (command->value == NULL) {
    return -1;
  }
  strncpy(command->value, str + comma_i + 1, i - comma_i + 1);

  return 0;
}

int main(int argc, char **argv) {
  struct kv_command *command_list[argc - 1];
  uint command_list_size = 0;

  for (int arg_i = 1; arg_i < argc; arg_i++) {
    char command = argv[arg_i][0];
    struct kv_command *curr_command = malloc(sizeof(struct kv_command));

    switch (command) {
    case 'p':
      curr_command->command = put; // kev, value
      break;
    case 'g':
      curr_command->command = get; // key
      break;
    case 'd':
      curr_command->command = delete; // key
      break;
    case 'c':
      curr_command->command = clear; // no value
      break;
    case 'a':
      curr_command->command = all; // no value
      break;
    default:
      fprintf(stderr, "Error: invalid operation %d:'%s'.\n", arg_i,
              argv[arg_i]);
      free(curr_command);
      continue;
    }

    if ((curr_command->command == all || curr_command->command == clear) &&
        argv[arg_i][1] != '\0') {
      fprintf(stderr, "Error: invalid query syntax %d:'%s'.\n", arg_i,
              argv[arg_i]);
      free(curr_command);
      continue;
    }
    if ((curr_command->command != all && curr_command->command != clear) &&
        argv[arg_i][1] != ',') {
      fprintf(stderr, "Error: invalid query syntax %d:'%s'.\n", arg_i,
              argv[arg_i]);
      free(curr_command);
      continue;
    }

    u_int8_t char_i = 2;

    if (curr_command->command == put) {
      if (parse_key_val_command(argv[arg_i] + 2, curr_command) == -1) {
        fprintf(stderr, "Error: invalid put query syntax %d:'%s'.\n", arg_i,
                argv[arg_i]);
        free(curr_command);
        continue;
      }
    } else {
      if (parse_key_command(argv[arg_i] + 2, curr_command) == -1) {
        fprintf(stderr, "Error: invalid query syntax %d:'%s'.\n", arg_i,
                argv[arg_i]);
        free(curr_command);
        continue;
      }
    }

    command_list[command_list_size++] = curr_command;
  }

  printf("Parsed commands\n");
  for (uint i = 0; i < command_list_size; i++) {
    printf("Command: %d: %s, %s\n", command_list[i]->command,
           command_list[i]->key, command_list[i]->value);
  }

  return 0;
}

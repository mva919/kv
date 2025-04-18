#ifndef PARSER_H
#define PARSER_H

typedef enum { put = 0, get = 1, del = 2, clear = 3, all = 4 } command_type;

struct kv_command {
  command_type command;
  char *key;
  char *value;
};

int parse_key_command(char *str, struct kv_command *command);
int parse_key_val_command(char *str, struct kv_command *command);
int parse_arguments(int argc, char **argv, struct kv_command **comman_list);

#endif // !PARSER_H

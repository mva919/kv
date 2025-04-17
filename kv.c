#include "parser.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_ssize_t.h>
#include <sys/_types/_u_int32_t.h>
#include <sys/_types/_u_int8_t.h>
#include <unistd.h>

#define BUF_SIZE 4096
#define MAX_COLLISION 5
#define INITIAL_MAP_SIZE 1000

struct hashmap_entry {
  int key;
  char *value;
};

// TODO: add file offset to make writing to files faster
struct hashmap_array {
  u_int8_t size;
  struct hashmap_entry ptr[MAX_COLLISION];
};

int main(int argc, char **argv) {
  struct kv_command *command_list[argc - 1];
  int comman_list_len = parse_arguments(argc, argv, command_list);

  int fd = open("./.kv_store", O_CREAT | O_RDWR);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  u_int32_t hashmap_size = 1000;
  struct hashmap_array hashmap[hashmap_size];
  memset(&hashmap, 0, sizeof hashmap);

  char buf[BUF_SIZE];

  hashmap[0].size++;
  hashmap[0].ptr[0].key = 1;
  hashmap[0].ptr[0].value = malloc(6);
  strcpy(hashmap[0].ptr[0].value, "Hello");

  // TODO: store size number in bits not bytes (micro optimization) - doing it
  // for keys would be better
  unsigned short buf_i = sprintf(buf, "%d", hashmap_size);

  buf[buf_i++] = ',';
  int bytes_to_write = buf_i;

  for (u_int32_t i = 0; i < hashmap_size; i++) {
    if (hashmap[i].size == 0) {
      continue;
    }

    for (u_int8_t j = 0; j < hashmap[i].size; j++) {
      // TODO: check that new data will not overflow buffer
      buf_i += sprintf(buf + buf_i, "%d,%s,", hashmap[i].ptr[j].key,
                       hashmap[i].ptr[j].value);
    }
  }

  int bytes_written = write(fd, buf, bytes_to_write + 1);
  lseek(fd, bytes_written, SEEK_CUR);

  return 0;
}

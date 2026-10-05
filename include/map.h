#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

typedef struct map {
  int length;
  char **values;
} map_t;

map_t *map_create(int length);

void map_destroy(map_t *m);

bool map_insert(map_t *m, char *key, char *value);

bool map_get(map_t *m, char *key, char **result);

#endif

#include "../include/map.h"
#include <stdlib.h>

// https://stackoverflow.com/a/7666577
static size_t hash_key(map_t *m, char *key) {
  unsigned long hash = 5381;
  int c = -1;

  while (c != '\0') {
    c = *key++;
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash % m->length;
}

map_t *map_create(int length) {
  map_t *new_map = malloc(sizeof(map_t));

  new_map->length = length;
  new_map->values = (char **)calloc(length, sizeof(char *));

  return new_map;
}

void map_destroy(map_t *m) { free(m); }

bool map_insert(map_t *m, char *key, char *value) {
  int hashed_key = hash_key(m, key);

  // Not empty
  if (m->values[hashed_key] != NULL) {
    return false;
  }

  m->values[hashed_key] = value;
  return true;
}

bool map_get(map_t *m, char *key, char **result) {
  int hashed_key = hash_key(m, key);

  // Empty
  if (m->values[hashed_key] == NULL) {
    result = NULL;
    return false;
  }

  *result = m->values[hashed_key];

  return true;
}

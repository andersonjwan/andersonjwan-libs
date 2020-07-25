#ifndef HASH_H
#define HASH_H

typedef struct {
  char *key;   // item key
  void *value; // item value
} item;

typedef struct {
  unsigned int size;  // max table size
  unsigned int count; // number of items in table currently

  char **items;       // array of pointers to item(s)
} table;

#endif

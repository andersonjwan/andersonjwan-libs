#ifndef HASH_H
#define HASH_H

/* standard libarary header(s) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* RESTRICTION(S) */
#define TABLE_SIZE 100

/* data structure definition(s) */
typedef struct {
  char *key;   // item key
  void *value; // item value
} hash_item;

typedef struct {
  unsigned int size;  // max table size
  unsigned int count; // number of items in table currently

  hash_item **items;  // array of pointers to item(s)
} hash_table;

#endif

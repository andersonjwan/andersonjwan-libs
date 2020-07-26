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

/* forward declaration(s) */
hash_table *new_hash_table(void);
static hash_item * new_hash_item(const char *, void *);
static void * del_hash_item(hash_item *);

static void check_alloc(const void *);

/* function definition(s) */
hash_table * new_hash_table(void) {
  hash_table *new_table;
  new_table = (hash_table *) malloc(sizeof(hash_table));
  check_alloc((void *) new_table);

  new_table->size  = TABLE_SIZE;
  new_table->count = 0;

  new_table->items = calloc((size_t) TABLE_SIZE, sizeof(hash_item *));
  check_alloc((void *) (new_table->items));

  return new_table;
}

static hash_item * new_hash_item(const char *key, void *value) {
  hash_item *new_item;
  new_item = (hash_item *) malloc(sizeof(hash_item));

  new_item->key   = strdup(key);
  new_item->value = value;

  return new_item;
}

static void * del_hash_item(hash_item *item) {
  void *value;
  value = item->value;

  free(item->key);
  free(item);

  return value;
}

static void check_alloc(const void *ptr) {
  if(ptr == NULL) {
    fprintf(stderr, "Error : Insufficient Memory\n");
    exit(1);
  }

  return;
}

#endif

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

  void (*garbage) (void *value);
  hash_item **items;  // array of pointers to item(s)
} hash_table;

/* global(s) */
static hash_item HASH_ITEM_DEL = {NULL, NULL};

/* forward declaration(s) */
hash_table * new_hash_table(void (*garbage) (void *));
void del_hash_table(hash_table *);
static hash_item * new_hash_item(const char *, void *);
static void * del_hash_item(hash_table *, hash_item *);

static int hash_index(const char *);
static int get_hash_index(const char *, int);

void hash_insert(hash_table *, const char *, void *);
void * hash_search(hash_table *, const char *);
void hash_delete(hash_table *, const char *);
void hash_print(hash_table *);

static void print_event(const char *);
static void check_alloc(const void *);

/* function definition(s) */
hash_table * new_hash_table(void (*garbage) (void *value)) {
  hash_table *new_table;
  new_table = (hash_table *) malloc(sizeof(hash_table));
  check_alloc((void *) new_table);

  new_table->size  = TABLE_SIZE;
  new_table->count = 0;
  new_table->garbage = garbage;  // set garbage collector function

  new_table->items = calloc((size_t) TABLE_SIZE, sizeof(hash_item *));
  check_alloc((void *) (new_table->items));

  return new_table;
}

void del_hash_table(hash_table *table) {
  for(int i = 0; i < table->size; ++i) {
    hash_item *item;
    item = table->items[i];

    if(item != NULL) {
      del_hash_item(table, item);
    }
  }

  free(table->items);
  free(table);
}

static hash_item * new_hash_item(const char *key, void *value) {
  hash_item *new_item;
  new_item = (hash_item *) malloc(sizeof(hash_item));

  new_item->key   = strdup(key);
  new_item->value = value;

  return new_item;
}

static void * del_hash_item(hash_table *table, hash_item *item) {
  void *value;
  value = item->value;

  free(item->key);
  free(item);

  (*(table->garbage)) (value);
}

static int hash_index(const char *key) {
  unsigned long hash = 5381;
  int character;

  while(character = *(key++)) {
    hash = ((hash << 5) + hash) + character;
  }

  return (int) (hash % TABLE_SIZE); // fit index into table
}

static int get_hash_index(const char *key, const int attempt) {
  const int hash = hash_index(key);
  return (hash + (attempt * (hash + 1))) % TABLE_SIZE;
}

void hash_insert(hash_table *table, const char *key, void *value) {
  hash_item *item;
  item = new_hash_item(key, value);

  int index;
  index = get_hash_index(item->key, 0);

  hash_item *curr;
  curr = table->items[index];
  int i = 1;

  /* collision handling : open addressing */
  while(curr != NULL) {
    /* update item's value */
    if(strcmp(curr->key, key) == 0) {
      del_hash_item(table, curr);

      /* update table */
      table->items[index] = item;
      return;
    }

    index = get_hash_index(item->key, i);
    print_event("Collision Detected");

    curr = table->items[index];
    ++i;
  }

  table->items[index] = item;
  ++(table->count);
}

void * hash_search(hash_table *table, const char *key) {
  int index;
  index = get_hash_index(key, 0);

  hash_item *item;
  item = table->items[index];

  int i = 1;
  while(item != NULL) {
    if(strcmp(item->key, key) == 0) {
      return item->value;
    }

    index = get_hash_index(key, i);
    item = table->items[index];
    ++i;
  }

  return NULL;
}

void hash_delete(hash_table *table, const char *key) {
  int index;
  index = get_hash_index(key, 0);

  hash_item *item;
  item = table->items[index];

  int i = 1;
  while(item != NULL) {
    if(item != &HASH_ITEM_DEL) {
      if(strcmp(item->key, key) == 0) {
        del_hash_item(table, item);
        table->items[index] = &HASH_ITEM_DEL;
      }
    }

    index = get_hash_index(key, i);
    item = table->items[index];
    ++i;
  }

  --(table->count);
}

void hash_print(hash_table *table) {
  hash_item *item;
  int active = 1;

  for(int i = 0; i < table->size; ++i) {
    item = table->items[i];

    if(item != NULL) {
      printf("Item #%02d @ Index %d\n", active, i);
      printf("Data: {%s, %p}\n", item->key, (char *) item->value);
      ++active;
    }
  }
}

static void print_event(const char *event) {
  fprintf(stdout, "\033[0;33m");    // set the text color to bold yellow
  fprintf(stdout, "EVENT");
  fprintf(stdout, "\033[0m");       // reset the text color to default

  fprintf(stdout, ": %s\n", event); // print message
}

static void check_alloc(const void *ptr) {
  if(ptr == NULL) {
    fprintf(stderr, "Error : Insufficient Memory\n");
    exit(1);
  }

  return;
}

#endif

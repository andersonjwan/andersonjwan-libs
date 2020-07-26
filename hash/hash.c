#include "hash.h"

struct Date {
  char *format;
};

void destroy(void *value) {
  printf("[enter] destroy\n");

  struct Date *tmp;
  tmp = (struct Date *) value;

  free(tmp->format);
  free(tmp);
  printf("[exit] destroy\n");
}

int main(void) {
  hash_table *table;
  table = new_hash_table();

  /*
  struct Date *date;
  date = (struct Date *) malloc(sizeof(struct Date));

  date->format = (char *) malloc(sizeof(char) * 10);
  strcpy(date->format, "07/25/20");

  hash_insert(table, "date", date);
  hash_print(table);

  hash_delete(table, "date", destroy);
  hash_print(table);
  */
  return 0;
}

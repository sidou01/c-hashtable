#include <stdlib.h>
#include <string.H>

#include "hash_table.h"

/*
   Allocate memory for a hash table item
   Assign the key and the value
   Return the newly created hash table item
*/
static ht_item* ht_new_item(const char* k, const char* v)
{
  ht_item* i = malloc(sizeof(ht_item));
  i->key = strdup(k);
  i->value = strdup(v);
  return i;
}

/*
   Allocate memory for a hash table
   Init it's size to 53 elements
   Allocate 53 memory slots for ptr to hash table item and set them to NULL (calloc)
*/

ht_hash_table* ht_new()
{
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));

  ht->size = 53;
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  return ht;
}

/*
   Deallocate a hash table item
*/
static void ht_del_item(ht_item* i)
{
  free(i->key);
  free(i->value);
  free(i);
}

/*
   Deallocate a hash table
*/
static void ht_del_hash_table(ht_hash_table* ht)
{
  for(int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if(item != NULL) {
      ht_del_item(item);
    }
  }

  free(ht->items);
  free(ht);

}

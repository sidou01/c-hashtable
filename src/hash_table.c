#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"

static const int HT_PRIME_1 = 151;
static const int HT_PRIME_2 = 163;
static ht_item HT_DELETED_ITEM = {NULL, NULL};

/*
   Allocate memory for a hash table item
   Assign the key and the value
   Return the newly created hash table item
*/
static ht_item *ht_new_item(const char *k, const char *v)
{
    ht_item *i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

/*
   Allocate memory for a hash table
   Init it's size to 53 elements
   Allocate 53 memory slots for ptr to hash table item and set them to NULL (calloc)
*/

ht_hash_table *ht_new()
{
    ht_hash_table *ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
    return ht;
}

/*
   Deallocate a hash table item
*/
static void ht_del_item(ht_item *i)
{
    free(i->key);
    free(i->value);
    free(i);
}

/*
   Deallocate a hash table
*/
void ht_del_hash_table(ht_hash_table *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        ht_item *item = ht->items[i];
        if (item != NULL)
        {
            ht_del_item(item);
        }
    }

    free(ht->items);
    free(ht);
}

/*
 * Hash function
 */

static int ht_hash(const char *s, const int a, const int m)
{
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++)
    {
        hash += (long)pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

/*
 * index = (hash_a(string) + i * (hash_b(string) + 1)) % num_buckets
 * i: number of hash collisions
 */

static int ht_get_hash(const char *s, const int num_buckets, const int attempt)
{
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

/*
 * Insert a new entry to the hash table
 */

void ht_insert(ht_hash_table *ht, const char *key, const char *value)
{
    ht_item *item = ht_new_item(key, value);
    int hash = ht_get_hash(item->key, ht->size, 0);
    ht_item *cur_item = ht->items[hash];

    int i = 0;
    if (cur_item != NULL && cur_item != &HT_DELETED_ITEM)
    {
        if (strcmp(cur_item->key, key) == 0)
        {
            ht_del_item(cur_item);
            ht->items[hash] = item;
            return;
        }
        hash = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[hash];
        i++;
    }
    ht->items[hash] = item;
    ht->count++;
}

/*
 * Search for a key in the hash table
 */
char *ht_search(ht_hash_table *ht, const char *key)
{
    int hash = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[hash];

    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
            if (strcmp(item->key, key) == 0)
                return item->value;

        hash = ht_get_hash(key, ht->size, i);
        item = ht->items[hash];
        i++;
    }
    return NULL;
}

/*
 * Delete an entry from a hash table given it's key
 */

void ht_delete(ht_hash_table *ht, const char *key)
{
    int hash = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[hash];

    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
        {
            if (strcmp(item->key, key) == 0)
            {
                ht_del_item(item);
                ht->items[hash] = &HT_DELETED_ITEM;
            }
        }
        hash = ht_get_hash(key, ht->size, i);
        item = ht->items[hash];
        i++;
    }
    ht->count--;
}
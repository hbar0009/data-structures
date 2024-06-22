#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define MAX_STR_SIZE 50

static const int polynomial_base = 31;
static const float max_fill = 0.5;

typedef struct KeyValuePair {
    char* key;
    char* val;
    bool deleted;
} keyval;

typedef struct HashMap {
    unsigned int capacity;
    unsigned int num_elems;
    keyval** values;
} hashmap;

keyval* new_keyval(char key[], char val[]);
void del_keyval(keyval* kv);

hashmap* new_hashmap();
void del_hashmap(hashmap* map);

int hash(char word[], int table_size);
void print_map(hashmap* map);
void resize(hashmap* map);
int find_index_of_key_or_empty(char key[], hashmap* map);
void insert(char key[], char val[], hashmap* map);
void delete(char key[], hashmap* map);
char* get(char key[], hashmap* map);

int main()
{
    printf("Intializing map\n");
    hashmap* map = new_hashmap();

    printf("Inserting into map\n");
    for (int i = 0; i < 6; ++i) {
        char key[10];
        char val[10];

        snprintf(key, 10, "%i", i);
        snprintf(val, 10, "%i", i);

        insert(key, val, map);
    }

    printf("inserting duplicate\n");
    insert("1", "blah", map);

    printf("Printing map\n");
    print_map(map);

    printf("Deleting hashmap\n");
    del_hashmap(map);

    printf("End of program\n");
    return 0;
}

keyval* new_keyval(char key[], char val[])
{
    keyval* kv = malloc(sizeof(keyval));
    kv->key = malloc(MAX_STR_SIZE * sizeof(char));
    kv->val = malloc(MAX_STR_SIZE * sizeof(char));

    strcpy(kv->key, key);
    strcpy(kv->val, val);
    kv->deleted = false;

    return kv;
}

void del_keyval(keyval* kv)
{
    free(kv->val);
    free(kv->key);
    free(kv);
}

hashmap* new_hashmap()
{
    hashmap* map = malloc(sizeof(hashmap));
    map->values = malloc(INITIAL_CAPACITY * sizeof(keyval*));
    map->num_elems = 0;
    map->capacity = INITIAL_CAPACITY;

    return map;
}

void del_hashmap(hashmap* map)
{
    for (int i = 0; i < map->capacity; ++i) {
        if (map->values[i]) {
            del_keyval(map->values[i]);
        }
    }

    free(map);
}

int hash(char word[], int table_size)
{
    int hash_val = 0;

    // TODO: maybe make use of unsigned integer overflow?
    for (int i = 0; i < strlen(word); i++) {
        hash_val += (long)((int)word[i] * pow(polynomial_base, i)) % table_size;
        hash_val %= table_size;
    }

    return hash_val;
}

void print_map(hashmap* map)
{
    for (int i = 0; i < map->capacity; i++) {
        if (map->values[i]) {
            printf("Position %i: (%s, %s)\n", i, map->values[i]->key, map->values[i]->val);
        } else {
            printf("Position %i: (null)\n", i);
        }
    }
}

void resize(hashmap* map)
{
    int old_capacity = map->capacity;
    map->capacity *= 2; // TODO: just arbitrarily doubling the size for now

    keyval** old_arr = map->values;

    map->values = malloc(map->capacity * sizeof(keyval*));

    keyval* pair;
    int insert_index;

    for (int i = 0; i < old_capacity; i++) {
        if ((pair = old_arr[i])) {
            insert_index = find_index_of_key_or_empty(pair->key, map);
            map->values[insert_index] = pair;
        }
    }
}

int find_index_of_key_or_empty(char key[], hashmap* map)
{
    // TODO: different probing strategies, for now we will use quadratic

    int hash_val = hash(key, map->capacity);

    int i = 0, first_tombstone = -1, search_index = hash_val;
    keyval* index_val;

    // search until we find an empty spot
    while (map->values[search_index]) {
        search_index = (hash_val + i * i) % map->capacity;

        index_val = map->values[search_index];

        if (!index_val) {
            break;
        }

        // if we find the key
        if (strcmp(index_val->key, key) == 0) {
            return search_index;
        }

        // the first time we find a tombstone, take note of its index
        if (index_val->deleted && first_tombstone != -1) {
            first_tombstone = search_index;
        }

        ++i;
    }

    if (first_tombstone != -1) {
        return first_tombstone;
    }

    return search_index;
}

void insert(char key[], char val[], hashmap* map)
{
    // if insertion would make this over capacity limit, resize
    if (map->num_elems + 1 >= map->capacity * max_fill) {
        resize(map);
    }

    int insert_index = find_index_of_key_or_empty(key, map);

    if (!map->values[insert_index]) {
        map->values[insert_index] = new_keyval(key, val);
        map->num_elems += 1;
    } else {
        strcpy(map->values[insert_index]->val, val);
    }
}

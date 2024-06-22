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
    keyval* values[INITIAL_CAPACITY]; // NOTE: should i be passing a pointer?
} hashmap;

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
    hashmap map = { INITIAL_CAPACITY };

    printf("Inserting into map\n");
    for (int i = 0; i < 10; ++i) {
        char key[10];
        char val[10];

        snprintf(key, 10, "%i", i);
        snprintf(val, 10, "%i", i);

        insert(key, val, &map);
    }

    printf("Printing map\n");
    print_map(&map);

    printf("End of program\n");
    return 0;
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

        // if we find the key, we will update it
        if (index_val->key == key) {
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

    map->values[insert_index] = malloc(sizeof(keyval));
    map->values[insert_index]->key = malloc(MAX_STR_SIZE * sizeof(char));
    map->values[insert_index]->val = malloc(MAX_STR_SIZE * sizeof(char));

    strcpy(map->values[insert_index]->key, key);
    strcpy(map->values[insert_index]->val, val);
    map->values[insert_index]->deleted = false;
}

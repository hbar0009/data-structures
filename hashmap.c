#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const int polynomial_base = 31;
// static const float max_fill = 0.5;

typedef struct KeyValuePair {
    char* key;
    char* val;
} keyval;

typedef struct HashMap {
    unsigned int capacity;
    keyval values[10]; // NOTE: should i be passing a pointer?
} hashmap;

int hash(char word[], int table_size);
void print_map(hashmap map);
void insert(char key[], char val[], hashmap map);
void delete(char key[], hashmap map);
char* get(char key[], hashmap map);
void resize(hashmap map);

int main()
{
    printf("Start of program\n");
    int initial_capacity = 10;
    keyval arr[10];

    printf("Intializing map\n");
    hashmap map = { initial_capacity };

    printf("Size of (keyval): %lu\n", sizeof(keyval));
    printf("Size of (*arr): %lu\n", sizeof(*arr));
    printf("Size of (arr): %lu\n", sizeof(arr));

    printf("Copying into map values\n");
    memcpy(map.values, arr, sizeof(*arr));

    printf("Printing map\n");
    print_map(map);

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

void print_map(hashmap map)
{
    for (int i = 0; i < map.capacity; i++) {
        printf("Position %i: (%s, %s)\n", i, map.values[i].key, map.values[i].val);
    }
}

void insert(char key[], char val[], hashmap map)
{
    // int hash_val = hash(key, map.capacity);
    // // NOTE: do i need to malloc this?
    // keyval pair = { key, val };
    // map.values[hash_val] = pair;
}

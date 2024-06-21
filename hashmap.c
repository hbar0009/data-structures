#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int polynomial_base = 31;
static const float max_fill = 0.5;

struct KeyValuePair {
    char* key;
    char* val;
};

struct HashMap {
    unsigned int capacity;
    struct KeyValuePair values[]; // NOTE: should i be passing a pointer?
};

void resize(int old_len, char* old[], int new_len, char* new[]);
int hash(char word[], int table_size);
void print_array(int len, char* arr[]);
void print_map(struct HashMap map);
void insert(char key[], char val[], struct HashMap map);
void delete(char key[], struct HashMap map);
char* get(char key[], struct HashMap map);
void resizeM(struct HashMap map);

int main()
{
    char** arr = malloc(10 * sizeof(char*));
    arr[hash("first", 10)] = "first";
    arr[hash("second", 10)] = "second";
    arr[hash("third", 10)] = "third";

    char** arr2 = malloc(20 * sizeof(char*));

    resize(10, arr, 20, arr2);

    free(arr);

    print_array(20, arr2);

    free(arr2);

    return 0;
}

void resize(int old_len, char* old[], int new_len, char* new[])
{
    for (int i = 0; i < old_len; i++) {
        // FIXME: this does not check for collisons
        if (old[i]) {
            new[hash(old[i], new_len)] = old[i];
        }
    }
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

void print_array(int len, char* arr[])
{
    for (int i = 0; i < len; i++) {
        printf("Element %i: %s\n", i, arr[i]);
    }
}

void print_map(struct HashMap map)
{
    for (int i = 0; i < map.capacity; i++) {
        struct KeyValuePair pair = map.values[i];
        printf("Position %i: (%s, %s)\n", i, pair.key, pair.val);
    }
}

void insert(char key[], char val[], struct HashMap map)
{
    int hash_val = hash(key, map.capacity);
    // NOTE: do i need to malloc this?
    struct KeyValuePair pair = { key, val };
    map.values[hash_val] = pair;
}

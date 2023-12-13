//
// Created by cristian on 13.12.2023.
//

#include "custom_c_hashset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../../custom_libraries/custom_c_logger/custom_c_logger.h"

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

unsigned long GetHashCode(const char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = (hash << 5) + hash + c;
    }

    return hash;
}

struct HashSet* InitHashSet() {
    struct HashSet* set = malloc(sizeof(struct HashSet));
    if (set == NULL) {
        LogError("Memory allocation for url set error");
        exit(EXIT_FAILURE);
    }

    set->array = (char**)calloc(INITIAL_CAPACITY, sizeof(char*));
    if (set->array == NULL) {
        LogError("Memory allocation for url set error");
        exit(EXIT_FAILURE);
    }

    set->size = 0;
    set->capacity = INITIAL_CAPACITY;

    return set;
}

void ResizeHashSet(struct HashSet* set) {
    const size_t new_capacity = set->capacity * 2;
    char** new_array = calloc(new_capacity, sizeof(char*));
    if (new_array == NULL) {
        LogError("Memory allocation for url set error");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < set->capacity; i++) {
        if (set->array[i] != NULL) {
            const unsigned long hash = GetHashCode(set->array[i]);
            size_t index = hash % new_capacity;

            while (new_array[index] != NULL) {
                index = (index + 1) % new_capacity;
            }

            new_array[index] = set->array[i];
        }
    }

    free(set->array);
    set->array = new_array;
    set->capacity = new_capacity;
}

void AddToHashSet(struct HashSet* set, const char* key) {
    if ((double)set->size / set->capacity > LOAD_FACTOR) {
        ResizeHashSet(set);
    }

    const unsigned long hash = GetHashCode(key);
    size_t index = hash % set->capacity;

    while (set->array[index] != NULL) {
        if (strcmp(set->array[index], key) == 0) {
            return;
        }
        index = (index + 1) % set->capacity;
    }

    set->array[index] = strdup(key);
    set->size++;
}

int IsInHashSet(const struct HashSet* set, const char* key) {
    const unsigned long hash = GetHashCode(key);
    size_t index = hash % set->capacity;

    while (set->array[index] != NULL) {
        if (strcmp(set->array[index], key) == 0) {
            return 1;
        }
        index = (index + 1) % set->capacity;
    }

    return 0;
}

void FreeHashSet(struct HashSet* set) {
    for (size_t i = 0; i < set->capacity; i++) {
        free(set->array[i]);
    }

    free(set->array);
    free(set);
}
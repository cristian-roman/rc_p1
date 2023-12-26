//
// Created by cristian on 13.12.2023.
//

#ifndef CUSTOM_C_HASHSET_H
#define CUSTOM_C_HASHSET_H
#include <stddef.h>

struct HashSet{
    char** array;
    size_t size;
    size_t capacity;
};

struct HashSet* InitHashSet();
void AddToHashSet(struct HashSet* set, const char* key);
int IsInHashSet(const struct HashSet* set, const char* key);
void FreeHashSet(struct HashSet* set);

#endif //CUSTOM_C_HASHSET_H

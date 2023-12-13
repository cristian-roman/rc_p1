//
// Created by cristian on 12.12.2023.
//

#include "url_table.h"

#include <stdlib.h>
#include <string.h>

#include "../../algorithms/custom_c_hash_set/custom_c_hashset.h"

struct UrlTable* InitUrlTable(const char* url, const int depth, const int max_depth) {
    struct UrlTable* url_table = malloc(sizeof(struct UrlTable));

    url_table->url_table = malloc(sizeof(char**) * (max_depth+1)); // dimension for first depth
    url_table->url_table[depth] = malloc(sizeof(char*)); // dimension for first url pointer
    url_table->url_table[depth][0] = malloc(sizeof(char) * strlen(url));  // dimension for first url
    strcpy(url_table->url_table[depth][0], url);

    url_table->read_url_count = malloc(sizeof(int) * (max_depth+1));
    bzero(url_table->read_url_count, sizeof(int) * (max_depth+1));
    url_table->read_url_count[depth] = 0;

    url_table->total_url_count = malloc(sizeof(int) * (max_depth+1));
    bzero(url_table->total_url_count, sizeof(int) * (max_depth+1));
    url_table->total_url_count[depth] = 1;

    url_table->max_depth = max_depth;

    url_table->url_set = InitHashSet();

    return url_table;
}

void AddUrlToTable(const struct UrlTable* url_table, const char* new_url, const int depth) {
    if(depth > url_table->max_depth) {
        return;
    }

    if(IsInHashSet(url_table->url_set, new_url)) {
        return;
    }

    if(url_table->total_url_count[depth] == 0) {
        url_table->url_table[depth] = malloc(sizeof(char*));
    }

    url_table->total_url_count[depth] += 1;

    url_table->url_table[depth] = realloc(url_table->url_table[depth], sizeof(char*) * url_table->total_url_count[depth]);
    url_table->url_table[depth][url_table->total_url_count[depth] - 1] = malloc(sizeof(char) * strlen(new_url));
    strcpy(url_table->url_table[depth][url_table->total_url_count[depth] - 1], new_url);

    AddToHashSet(url_table->url_set, new_url);
}

void FreeUrlTable(struct UrlTable* url_table) {
    for(int i = 0; i < url_table->max_depth; i++) {
        for(int j = 0; j < url_table->total_url_count[i]; j++) {
            free(url_table->url_table[i][j]);
        }
        free(url_table->url_table[i]);
    }
    free(url_table->read_url_count);
    free(url_table->total_url_count);
    free(url_table->url_table);
    FreeHashSet(url_table->url_set);
    free(url_table);
}

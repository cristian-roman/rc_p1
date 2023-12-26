//
// Created by cristian on 12.12.2023.
//

#include "url_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../algorithms/custom_c_hash_set/custom_c_hashset.h"
#include "../../../../custom_libraries/custom_c_logger/custom_c_logger.h"
#include "../../../../custom_libraries/custom_c_string/custom_c_string.h"

struct UrlTable* InitUrlTable(const char* url, const int depth, const int max_depth) {
    struct UrlTable* url_table = malloc(sizeof(struct UrlTable));

    url_table->url_table = malloc(sizeof(char**) * (max_depth+1));
    url_table->url_table[depth] = malloc(sizeof(char*));

    const int url_length = strlen(url);

    url_table->url_table[depth][0] = calloc(url_length + 10, sizeof(char));
    strncpy(url_table->url_table[depth][0], url, url_length);
    EnsureNullOverTheBuffer(url_table->url_table[depth][0], url_length);

    url_table->read_url_count = malloc(sizeof(int) * (max_depth+1));
    bzero(url_table->read_url_count, sizeof(int) * (max_depth+1));
    url_table->read_url_count[depth] = 0;

    url_table->total_url_count = malloc(sizeof(int) * (max_depth+1));
    bzero(url_table->total_url_count, sizeof(int) * (max_depth+1));
    url_table->total_url_count[depth] = 1;

    url_table->max_depth = max_depth;

    url_table->url_set = InitHashSet();

    AddToHashSet(url_table->url_set, url);

    LogUrlTable(url_table);

    return url_table;
}

void LogUrlTable(const struct UrlTable* url_table) {

    const char* starting_line = "Url table overview:\n";
    char* log_message = strdup(starting_line);

    for(int i = 0; i <= url_table->max_depth; i++) {
        const char* line_message_pattern = "For depth %d: %d/%d urls read. Urls left to be read:\n";
        char* line_message = calloc(strlen(line_message_pattern) + 30, sizeof(char));
        sprintf(line_message, line_message_pattern, i, url_table->read_url_count[i], url_table->total_url_count[i]);
        char* tmp_message = CombineStrings(2, strlen(log_message) + strlen(line_message) + 10, log_message, line_message);
        free(log_message);
        free(line_message);
        log_message = tmp_message;

        for(int j = url_table->read_url_count[i]; j < url_table->total_url_count[i]; j++) {
            const char* url_line_pattern = "- %s\n";
            char* url_line = calloc(strlen(url_table->url_table[i][j]) + 10, sizeof(char));
            sprintf(url_line, url_line_pattern, url_table->url_table[i][j]);
            tmp_message = CombineStrings(2, strlen(log_message) + strlen(url_line) + 10, log_message, url_line);
            free(log_message);
            free(url_line);
            log_message = tmp_message;
        }

    }

    LogInfo(log_message);
    free(log_message);
}


void AddUrlToTable(const struct UrlTable* url_table, const char* new_url, const int depth) {
    if(depth > url_table->max_depth) {
        return;
    }

    if(IsInHashSet(url_table->url_set, new_url)) {
        return;
    }

    if(url_table->total_url_count[depth] == 0) {
        url_table->url_table[depth] = calloc(1,sizeof(char*));
    }

    url_table->total_url_count[depth] += 1;

    url_table->url_table[depth] = realloc(url_table->url_table[depth], sizeof(char*) * url_table->total_url_count[depth]);
    const int new_url_length = strlen(new_url);
    url_table->url_table[depth][url_table->total_url_count[depth] - 1] = calloc(new_url_length + 10, sizeof(char));
    strncpy(url_table->url_table[depth][url_table->total_url_count[depth] - 1], new_url, new_url_length);
    EnsureNullOverTheBuffer(url_table->url_table[depth][url_table->total_url_count[depth] - 1], new_url_length);

    AddToHashSet(url_table->url_set, new_url);

    const char* pattern = "Added url %s to url table";
    char* log_message = calloc(strlen(pattern) + strlen(new_url) + 10, sizeof(char));
    sprintf(log_message, pattern, new_url);
    LogInfo(log_message);
    free(log_message);
}

void FreeUrlTable(struct UrlTable* url_table) {
    for(int i = 0; i <= url_table->max_depth; i++) {
        for(int j = 0; j < url_table->total_url_count[i]; j++) {
            free(url_table->url_table[i][j]);
        }
        if(url_table->total_url_count[i] > 0)
            free(url_table->url_table[i]);
    }
    free(url_table->read_url_count);
    free(url_table->total_url_count);
    free(url_table->url_table);
    FreeHashSet(url_table->url_set);
    free(url_table);
}

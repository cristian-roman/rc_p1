//
// Created by cristian on 12.12.2023.
//

#ifndef URL_TABLE_H
#define URL_TABLE_H

struct UrlTable {
    char*** url_table;
    int* read_url_count;
    int* total_url_count;
    int max_depth;
    struct HashSet* url_set;
};

struct UrlTable* InitUrlTable(const char* url, int depth, int max_depth);
void AddUrlToTable(const struct UrlTable* url_table, const char* new_url, const int depth);
void FreeUrlTable(struct UrlTable* url_table);

#endif //URL_TABLE_H

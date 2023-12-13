//
// Created by cristian on 12.12.2023.
//

#ifndef URL_EXTRACTOR_H
#define URL_EXTRACTOR_H
#include "../file_hierarchy_creator/file_hierarchy_creator.h"
#include "../url_table/url_table.h"

void AddResourcesToUrlTable(const struct UrlTable* url_table, const struct Folder_Resource_Pair* folder_resource_pair, const char* url);
#endif //URL_EXTRACTOR_H

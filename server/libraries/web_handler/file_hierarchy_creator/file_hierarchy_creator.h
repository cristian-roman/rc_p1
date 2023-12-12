//
// Created by cristian on 12.12.2023.
//

#ifndef FILE_HIERARCHY_CREATOR_H
#define FILE_HIERARCHY_CREATOR_H

void CreateFolder(const char* path, const char* folder_name);
void CreateHierarchyFromUrl(const char* starting_location, const char* url);
char* GetPathToResource(const char* starting_location, const char* url);

#endif //FILE_HIERARCHY_CREATOR_H

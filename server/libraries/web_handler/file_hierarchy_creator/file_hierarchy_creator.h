//
// Created by cristian on 12.12.2023.
//

#ifndef FILE_HIERARCHY_CREATOR_H
#define FILE_HIERARCHY_CREATOR_H

struct Folder_Resource_Pair {
    char* folder;
    char* resource;
};

void CreateFolder(const char* path, const char* folder_name);
void CreateHierarchyFromUrl(const char* starting_location, const char* url);
struct Folder_Resource_Pair* GetPathToResource(const char* starting_location, const char* url);
void FreeFolderResourcePair(struct Folder_Resource_Pair* folder_resource_pair);

#endif //FILE_HIERARCHY_CREATOR_H

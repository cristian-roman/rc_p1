//
// Created by cristian.roman on 9/11/23.
//

#ifndef SERVER_MYSTRING_H
#define SERVER_MYSTRING_H

char* CombineStrings(int numStrings, int total_size, ...);
char* DuplicateString(const char* string);
void EnsureNullOverTheBuffer(char* string, int size);
#endif //SERVER_MYSTRING_H
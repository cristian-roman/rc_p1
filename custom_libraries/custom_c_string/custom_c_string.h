//
// Created by cristian.roman on 9/11/23.
//

#ifndef SERVER_MYSTRING_H
#define SERVER_MYSTRING_H

char* CombineStrings(int numStrings, int total_size, ...);
char* GetStringFromPattern(const char* pattern, const int total_size, ...);
int GetNumberOfTokens(const char* str, const char delimiter);
char** SplitString(char* str, char delimiter, int numTokens);
void FreeSplitString(char** tokens, int numTokens);

#endif //SERVER_MYSTRING_H
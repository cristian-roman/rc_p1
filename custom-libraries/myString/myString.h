//
// Created by cristian.roman on 9/11/23.
//

#ifndef SERVER_MYSTRING_H
#define SERVER_MYSTRING_H

void CombineStrings(char* result, int numStrings, ...);
char* IntegerToString(char* result, int number);
char** SplitString(char* str, char delimiter, int numTokens);

#endif //SERVER_MYSTRING_H
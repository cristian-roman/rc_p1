//
// Created by cristian on 13.12.2023.
//

#ifndef URL_HANDLER_H
#define URL_HANDLER_H

struct Tokens_Length_Pair {
    char** tokens;
    int length;
};

char* RemoveUrlPrefix(const char* url);
char* RemoveLastSlash(const char* url);
struct Tokens_Length_Pair* ExtractUrlTokens(const char* url);
void FreeTokensLengthPair(const struct Tokens_Length_Pair* tokens_length_pair);
int TokenHasExtension(const char* token);
char* GetUrlWithoutResource(const char* url);
char* GetBaseUrl(const char* url);
int GetUrlDepth(const char* url);
int IsUrl(const char* str);
#endif //URL_HANDLER_H

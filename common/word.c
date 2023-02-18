/*
* word - Helps with ensuring a word is big enough,
* has no numbers, and is all lowercase
* 
* Dylan Buchanan, February 2023
*/

#include <ctype.h>
#include <string.h>
#include <ctype.h>
#include "../libcs50/mem.h"

/***********normalizeWord()***********/
// see word.h for comments about exported functions
char* normalizeWord(const char* word) {
    // makes a copy of the word
    char* lower = mem_malloc_assert(strlen(word) * sizeof(char) + 1, "ERROR: Out of memory allocating lower case word");
    strcpy(lower, word);
    // ensures it is big enough
    if (strlen(lower) < 3) {
        mem_free(lower);
        return NULL;
    }
    // loop through each letter
    for (int i = 0; i < strlen(word); i++) {
        // ensure it is a letter
        if (!isalpha(lower[i])) {
            mem_free(lower);
            return NULL;
        }
        // make sure it is lower case
        lower[i] = tolower(lower[i]);
    }
    // return it
    return lower;
}

char* normalizeQuery(const char* word) {
    // makes a copy of the word
    char* lower = mem_malloc_assert(strlen(word) * sizeof(char) + 1, "ERROR: Out of memory allocating lower case word");
    strcpy(lower, word);
    // ensures it is big enough
    if (strlen(lower) < 2) {
        mem_free(lower);
        return NULL;
    }
    // loop through each letter
    for (int i = 0; i < strlen(word); i++) {
        // ensure it is a letter
        if (isspace(lower[i])) {
            continue;
        }
        if (!isalpha(lower[i])) {
            mem_free(lower);
            return NULL;
        }
        // make sure it is lower case
        lower[i] = tolower(lower[i]);
    }
    // return it
    return lower;
}
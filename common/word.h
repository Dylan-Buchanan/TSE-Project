/*
* word - Helps with ensuring a word is big enough,
* has no numbers, and is all lowercase
* 
* Dylan Buchanan, February 2023
*/

#include <ctype.h>
#include <string.h>
#include "../libcs50/mem.h"

/***********normalizeWord()***********/
/*
* Ensures a word is big enough (3 letters or more), has no numbers, 
* and is all lowercase
* Returns the word if all cases pass
* Returns NULL if the word is too small or has numbers in it
*/
char* normalizeWord(const char* word);

/***********normalizeQuery()***********/
/*
* Ensures a word is big enough (2 letters or more), has no numbers, 
* and is all lowercase
* Ignores Spaces
* Returns the word if all cases pass
* Returns NULL if the word is too small or has numbers in it
*/
char* normalizeQuery(const char* word);
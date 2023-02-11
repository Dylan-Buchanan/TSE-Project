/*
* index - Utility functions that rely on storing counters for words
* 
* Defines the index_t* type and functions for altering and accessing the index
* Holds a hashtable that takes a word and links it to a counter that stores
* docID and count pairs
*
* Dylan Buchanan, Februrary 2023
*/

#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/counters.h"

/*********** global type ***********/
typedef struct index index_t;

/***********index_new()***********/
/*
* Creates a new index object from an input of the max number of bins for the hashtable
* Allocates memory for it and returns the index_t
*
* Guarantees the index is initialized empty
* Returns NULL if memory issues occurred
*/
index_t* index_new(int size);

/***********index_find()***********/
/*
* Finds the counter for the specified key and returns it
*
* Takes an index object and a char* key as parameters
* Checks to see if the key exists by checking for a counters object
* If a key exists there will always be a counters object
*
* Returns NULL if nothing was found
* Returns the counters object if the key exists
*/
counters_t* index_find(index_t* index, char* key);

/***********index_update()***********/
/*
* Updates the count of a the number of times a word is in a document
*
* Takes an index object, a const char* key, and a const int docID as parameters
* Calls index_find() to see if the key exists
* 
* If it doesn't exist then it creates a counters object
* It adds the docID to the counters object (which increments to 1)
* And inserts the counters object into the index
*
* If the key does exist then the appropriate counters object
* for the key is obtained and incremented for the correct docID
*/
void index_update(index_t* index, const char* key, const int docID);

/***********index_save()***********/
/*
* Creates an index file in the format: word docID count docID count ...
* 
* Takes an index object and an indexFilename as parameters
* Checks to see if the parameters are valid
* Creates a file to write in if indexFilename is valid
*
* Iterate over the hashtable and print the key
* That iteration has a function passed in that iterates over the counters for that word
* The docIDs and counts are printed as well
*/
void index_save(index_t* index, const char* indexFilename);

/***********index_delete()***********/
/*
* Deletes the hashtable in index and frees the index
* Checks to see if the index passed in is valid
*/
void index_delete(index_t* index);

/***********index_set()***********/
/*
* Inputs a specific counter into an index for a specific word
* 
* Takes an index object, word, and counters object as parameters
* If any parameter is invalid it does not set the counter into the index
*/
void index_set(index_t* index, char* key, counters_t* ct);

/***********index_load()***********/
/*
* Takes an index filename, reads it, and turns it into an index object that is returned
*
* Checks to see if the file is able to opened
* Takes one line of the file
* Splits it by spaces and creates a counter
* The first item is the word
* Then in pairs of 2 (docID and count) those values are added to a counter
* Takes the word and the counters object and inputs into an index
*
* This continues until there are no more lines in the file
* Returns the index
*/
index_t* index_load(const char* oldIndexFilename);
/*
* index - Utility functions that rely on storing counters for words
* 
* Defines the index_t* type and functions for altering and accessing the index
* Holds a hashtable that takes a word and links it to a counter that stores
* docID and count pairs
*
* Dylan Buchanan, Februrary 2023
*/

#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../libcs50/counters.h"


static void counters_iterate_helper(void* file, const int docID, const int count);
static void index_save_helper(void* file, const char* key, void* item);
static void index_delete_helper(void* ct);

/*********** global type ***********/
typedef struct index {
    hashtable_t* hash;
} index_t;

/***********index_new()***********/
// see index.h for comments about exported functions
index_t* index_new(int size) {
    // allocate memory for index
    index_t* index = mem_malloc_assert(sizeof(index_t), "ERROR: Out of memory for index allocation\n");
    // create a new hashtable
    hashtable_t* ht = hashtable_new(size);
    if (ht == NULL) {
        fprintf(stderr, "ERROR: Out of memory for hashtable allocation\n");
        return NULL;
    }
    // puts the hashtable into the index
    index->hash = ht;
    return index;
}

/***********index_find()***********/
// see index.h for comments about exported functions
counters_t* index_find(index_t* index, const char* key) {
    // check to see if the index and key are valid
    if (index == NULL) {
        fprintf(stderr, "ERROR: No index object passed to index_find()\n");
        return NULL;
    }
    if (key == NULL) {
        fprintf(stderr, "ERROR: No key passed to index_find()\n");
        return NULL;
    }
    // return the counter for the key if it exists
    return hashtable_find(index->hash, key);
}

/***********index_update()***********/
// see index.h for comments about exported functions
void index_update(index_t* index, const char* key, const int docID) {
    // check to see if the arguments are valid
    if (index == NULL) {
        fprintf(stderr, "ERROR: No index object passed to index_update()\n");
        return;
    }
    if (key == NULL) {
        fprintf(stderr, "ERROR: No key passed to index_update()\n");
        return;
    }
    if (docID == 0) {
        fprintf(stderr, "ERROR: No docID passed to index_update()\n");
        return;
    }
    // find the counter if it exists
    counters_t* count = index_find(index, key);
    if (count == NULL) {
        // if it doesn't make a new one
        counters_t* ct = counters_new();
        if (ct == NULL) {
            fprintf(stderr, "ERROR: Out of memory for counter allocation\n");
            return; 
        }
        // add a new document to the counter since its new
        counters_add(ct, docID);
        // insert the counter into the hashtable
        hashtable_insert(index->hash, key, ct);
    }
    // otherwise simply increment the count for that document
    else {
        counters_add(count, docID);
    }
}

/***********counters_iterate_helper()***********/
// A helper function for index_save_helper
static void counters_iterate_helper(void* file, const int docID, const int count) {
    // print the docID and count
    fprintf(file, " %d %d", docID, count);
}

/***********index_save_helper()***********/
// A helper function for index_save
static void index_save_helper(void* file, const char* key, void* item) {
    // print the key
    fprintf(file, "%s", key);
    // iterate through the counter
    counters_iterate(item, file, counters_iterate_helper);
    // print the next line
    fprintf(file, "\n");
}

/***********index_save()***********/
// see index.h for comments about exported functions
void index_save(index_t* index, const char* indexFilename) {
    // check if the parameters are valid
    if (index == NULL) {
        fprintf(stderr, "ERROR: No index object passed to index_save()\n");
        return;
    }
    if (indexFilename == NULL) {
        fprintf(stderr, "ERROR: No indexFilename passed to index_save()\n");
        return;
    }
    // open the file to read
    FILE* file = fopen(indexFilename, "w");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Unable to open %s file or unable to write in it\n", indexFilename);
        return;
    }
    // iterate through the hashtable of the index
    hashtable_iterate(index->hash, file, index_save_helper);
    // close the file
    fclose(file);
}

/***********index_delete_helper()***********/
// A helper function for index_delete
static void index_delete_helper(void* ct) {
    // free the counter
    counters_delete(ct);
}

/***********index_delete()***********/
// see index.h for comments about exported functions
void index_delete(index_t* index) {
    // check if the index is valid
    if (index == NULL) {
        fprintf(stderr, "ERROR: No index object passed to index_delete()\n");
        return;
    }
    // free the hashtable
    hashtable_delete(index->hash, index_delete_helper);
    // free the index
    mem_free(index);
}

/***********index_set()***********/
// see index.h for comments about exported functions
void index_set(index_t* index, char* key, counters_t* ct) {
    // check if the parameters are valid
    if (index == NULL) {
        fprintf(stderr, "ERROR: No index object passed to index_set()\n");
        return;
    }
    if (key == NULL) {
        fprintf(stderr, "ERROR: No key passed to index_set()\n");
        return;
    }
    if (ct == NULL) {
        fprintf(stderr, "ERROR: No counters object passed to index_set()\n");
        return;
    }
    // insert the word and counters object into the hashtable
    hashtable_insert(index->hash, key, ct);
    
}

/***********index_load()***********/
// see index.h for comments about exported functions
index_t* index_load(const char* oldIndexFilename) {
    // open the file to read
    FILE* oldFile = fopen(oldIndexFilename, "r");
    // check that the file is valid
    if (oldFile == NULL) {
        fprintf(stderr, "ERROR: %s is not a valid filename", oldIndexFilename);
        return NULL;
    }
    // initialize an index
    index_t* index = index_new(200);
    // read the first line
    char* line = file_readLine(oldFile);
    // while there is a line to read
    while (line != NULL) {
        // initialize a counter
        counters_t* ct = counters_new();
        // split the line by spaces
        char* split = strtok(line, " ");
        // store the word
        char* word = mem_malloc_assert(strlen(split) * sizeof(char) + 1, "ERROR: Out of memory allocating word");
        strcpy(word, split);
        // move to the next item after the word and keep looking for docID/count pairs until the line is done
        split = strtok(NULL, " ");
        while (split != NULL) {
            int docID;
            int count;
            // get the docID and count
            sscanf(split, "%d", &docID);
            split = strtok(NULL, " ");
            sscanf(split, "%d", &count);
            // insert it into the counter and check for more pairs
            counters_set(ct, docID, count);
            split = strtok(NULL, " ");
        }
        // put the word and counter into the index
        index_set(index, word, ct);
        // free variables
        mem_free(word);
        mem_free(split);
        mem_free(line);
        // get the next line
        line = file_readLine(oldFile);
    }
    // free the line, close the file, and return the index
    mem_free(line);
    fclose(oldFile);
    return index;
}

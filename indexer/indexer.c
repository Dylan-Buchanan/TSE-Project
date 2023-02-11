/*
* indexer - Takes information from crawler.c and creates a index
* of the number of times words appear on certain websites
*
* Dylan Buchanan, Februrary 2023
*/

#include <stdio.h>
#include <string.h>
#include "../libcs50/mem.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"

static void indexPage(index_t* index, webpage_t* page, const int docID);
static void indexBuild(const char* pageDirectory, const char* indexFilename);

/***********main()***********/
/*
* Parses the command line, validate parameters, initialize other modules
* call indexBuild, with pageDirectory
*/
int main(const int argc, char* argv[]) {
    // ensure there are 2 arguments
    if (argc != 3) {
        fprintf(stderr, "Please input 2 arguments: pageDirectory indexFilename\n");
        exit(1);
    }
    // copy the arguments
    char* pageDirectory = mem_malloc_assert(strlen(argv[1]) * sizeof(char) + 1, "ERROR: Out of memory allocating pageDirectory\n");
    strcpy(pageDirectory, argv[1]);
    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "ERROR: %s is not a crawler directory\n", pageDirectory);
        mem_free(pageDirectory);
        exit(1);
    }
    char* indexFilename = mem_malloc_assert(strlen(argv[2]) * sizeof(char) + 1, "ERROR: Out of memory allocating indexFilename\n");
    strcpy(indexFilename, argv[2]);
    // try opening the indexFilename to write to ensure it can
    FILE* file = fopen(indexFilename, "w");
    if (file == NULL) {
        fprintf(stderr, "ERROR: %s is not a valid filename\n", indexFilename);
        mem_free(pageDirectory);
        mem_free(indexFilename);
        exit(1);
    }
    fclose(file);
    // build the index
    indexBuild(pageDirectory, indexFilename);
    // free the arguments
    mem_free(pageDirectory);
    mem_free(indexFilename);
    return 0;
}

/***********indexBuild()***********/
/*
* Creates the index file based on a crawler directory
*
* Creates a new 'index' object
* Loops over document ID numbers, counting from 1
* Loads a webpage from the document file 'pageDirectory/id'
*   
* If successful passes the webpage and docID to indexPage
*/
static void indexBuild(const char* pageDirectory, const char* indexFilename) {
    // intialize the index
    index_t* index = index_new(200);
    // start at doc 1
    int docID = 1;
    // load a webpage for that id
    webpage_t* page = pagedir_load(pageDirectory, docID);
    // as long as a webpage can be made
    while (page != NULL) {
        // count the words on that page
        printf("Checking Doc: %d\n", docID);
        indexPage(index, page, docID);
        docID++;
        // delete the webpage and get the next one
        webpage_delete(page);
        page = pagedir_load(pageDirectory, docID);
    }
    // save the index to the file and delete the index
    index_save(index, indexFilename);
    index_delete(index);
}

/***********indexPage()***********/
/*
* Goes through a page a counts the number of times a word appears in a document
*
* Steps through each word of the webpage and skips trivial words (less than length 3)
* Normalizes the word (converts to lower case)
* Looks up the word in the index adding the word to the index if needed
* Increments the count of occurrences of this word in this docID
*/
static void indexPage(index_t* index, webpage_t* page, const int docID) {
    // get all the words on the page
    int pos = 0;
    char* word = webpage_getNextWord(page, &pos);
    while (word != NULL) {
        // normalize the word
        char* lower = normalizeWord(word);
        if (lower != NULL) {
            // if the word normalized properly, increment its occurrence
            index_update(index, lower, docID);
        }
        // free the words
        mem_free(lower);
        mem_free(word);
        // get the next word
        word = webpage_getNextWord(page, &pos);
    }
    // clean up
    mem_free(word);
}
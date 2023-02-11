/*
* indextest - A test code for index.c and indexer.c
* 
* Takes an index file already created and the name for a new file
* and creates an index from the old file and puts it into the new file
*
* Dylan Buchanan, Februrary 2023
*/

#include <ctype.h>
#include <string.h>
#include "../libcs50/mem.h"
#include "../common/index.h"
#include "../libcs50/file.h"
#include "../libcs50/counters.h"

/***********main()***********/
/*
* Takes an index file already created and the name for a new file
* and creates an index from the old file and puts it into the new file
*
* Ensures number of arguments are correct
* Loads the index from the old file
* Saves the index to the new file
* 
* Returns 0
*/
int main(const int argc, char* argv[]) {
    // check correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Please input 2 arguments: oldIndexFilename newIndexFilename\n");
        exit(1);
    }
    // grab names from arguments
    char* oldIndexFilename = mem_malloc_assert(strlen(argv[1]) * sizeof(char) + 1, "ERROR: Out of memory allocating oldIndexFilename\n");
    strcpy(oldIndexFilename, argv[1]);
    char* newIndexFilename = mem_malloc_assert(strlen(argv[2]) * sizeof(char) + 1, "ERROR: Out of memory allocating newIndexFilename\n");
    strcpy(newIndexFilename, argv[2]);
    // load the index
    index_t* index = index_load(oldIndexFilename);
    // check that it loaded properly
    if (index == NULL) {
        fprintf(stderr, "ERROR: %s is an invalid filename\n", oldIndexFilename);
        mem_free(oldIndexFilename);
        mem_free(newIndexFilename);
        exit(1);
    }
    // save the index
    index_save(index, newIndexFilename);
    // delete the index and free the filenames
    index_delete(index);
    mem_free(oldIndexFilename);
    mem_free(newIndexFilename);
    return 0;
}
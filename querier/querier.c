/*
* querier - 
*
*
* Dylan Buchanan, February 2023
*/

#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/index.h"

/***********main()***********/
/*
* Parses arguments and loads the index from indexFilename
*
* Make sure there are only 2 arguments
* Make sure pageDirectory is a valid crawler directory
* Make sure that indexFilename exists
*
* Ask the user for input
*/
int main(const int argc, char* argv[]) {
    // check that the number of arguments is correct
    if (argc != 3) {
        fprintf(stderr, "Please input 2 arguments in the format: ./querier pageDirectory indexFilename\n");
        exit(1);
    }
    // check to see that the pageDirectory is valid
    char* pageDirectory = mem_malloc_assert(strlen(argv[1]) * sizeof(char) + 1, "ERROR: Out of memory allocating pageDirectory\n");
    strcpy(pageDirectory, argv[1]);
    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "ERROR: %s is not a crawler directory\n", pageDirectory);
        mem_free(pageDirectory);
        exit(1);
    }
    char* indexFilename = mem_malloc_assert(strlen(argv[2]) * sizeof(char) + 1, "ERROR: Out of memory allocating indexFilename\n");
    strcpy(indexFilename, argv[2]);
    // try opening the indexFilename to make sure it exists
    FILE* file = fopen(indexFilename, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: %s is not a valid filename\n", indexFilename);
        mem_free(pageDirectory);
        mem_free(indexFilename);
        exit(1);
    }
    // load the index
    index_t* index = index_load(indexFilename);
    mem_free(indexFilename);
    // ask the user for input
    askQuery(index, pageDirectory);
    // clean up
    mem_free(pageDirectory);
}

char* askQuery(index_t* index, const char* pageDirectory) {

}
/*
*
* pagedir.c - Adds 2 functions:
* 
* First function creates a .crawler file in the directory given
* and returns true or false depending if it could access the directory
*
* Second function creates a file and writes the information of the webpage
* passed into the function into that file
*
* Dylan Buchanan, Winter 2023
*/

#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"

bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/***********pagedir_init()***********/
/* Constructs the pathname for the .crawler file in that directory
*
*  Opens the file for writing; on error, return false.
*  Closes the file and return true.
*/
bool pagedir_init(const char* pageDirectory) {
    // allocate memory for the full file pathname and build the file pathname
    char* pathname = "/.crawler";
    char* fileMem = mem_malloc_assert((strlen(pageDirectory) + strlen(pathname)) * sizeof(char) + 1, "ERROR: failed allocating memory for .crawler file\n");
    sprintf(fileMem, "%s%s", pageDirectory, pathname);
    // open the file to write (if possible)
    FILE* file = fopen(fileMem, "w");
    // otherwise directory does not exist and return false
    if (file == NULL) {
        fprintf(stderr, "ERROR: .crawler unable to open\n");
        return false;
    }
    // close the file and free the pathname
    fclose(file);
    mem_free(fileMem);
    return true;
}

/***********pagedir_save()***********/
/* Constructs the pathname for the page file in pageDirectory
*  
*  Opens that file for writing, prints the URL, prints the depth,
*  prints the contents of the webpage
*
*  Closes the file
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    // find the size of the docID
    int idSize = snprintf(NULL, 0, "%d", docID) + 1;
    // allocate memory for the pathname and build the pathname
    char* fileMem = mem_malloc_assert((strlen(pageDirectory) + idSize) * sizeof(char) + 1, "ERROR: failed allocating memory for saving webpage\n");
    sprintf(fileMem, "%s/%d", pageDirectory, docID);
    // because the page directory was checked before we know the directory is valid so file will open
    FILE* file = fopen(fileMem, "w");
    // print page information to file
    fprintf(file, "%s\n", webpage_getURL(page));
    fprintf(file, "%d\n", webpage_getDepth(page));
    fprintf(file, "%s\n", webpage_getHTML(page));  
    // close the file and free the pathname 
    fclose(file);
    mem_free(fileMem);
}
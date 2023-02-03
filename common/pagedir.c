/*
*
* pagedir.c - 
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
    char* pathname = "/.crawler";
    char* fileMem = mem_malloc_assert((strlen(pageDirectory) + strlen(pathname)) * sizeof(char) + 1, "ERROR: failed allocating memory for .crawler file\n");
    sprintf(fileMem, "%s%s", pageDirectory, pathname);
    FILE* file = fopen(fileMem, "w");
    if (file == NULL) {
        fprintf(stderr, "ERROR: .crawler unable to open\n");
        return false;
    }
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
    int idSize = snprintf(NULL, 0, "%d", docID) + 1;
    char* fileMem = mem_malloc_assert((strlen(pageDirectory) + idSize) * sizeof(char) + 1, "ERROR: failed allocating memory for saving webpage\n");
    sprintf(fileMem, "%s/%d", pageDirectory, docID);
    FILE* file = fopen(fileMem, "w");
    fprintf(file, "%s\n", webpage_getURL(page));
    fprintf(file, "%d\n", webpage_getDepth(page));
    fprintf(file, "%s\n", webpage_getHTML(page));   
    fclose(file);
    mem_free(fileMem);
}
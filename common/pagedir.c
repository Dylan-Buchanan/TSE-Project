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

/***********pagedir_validate()***********/
/*
* Checks to see if a pageDirectory has a .crawler file in it 
* Returns true if it is a valid crawler directory
*/
bool pagedir_validate(const char* pageDirectory) {
    // copy the directory name to look at .crawler
    char* crawler = mem_malloc_assert((strlen(pageDirectory) + strlen("/.crawler")) * sizeof(char) + 1, "ERROR: Out of memory allocating crawler file location");
    sprintf(crawler, "%s%s", pageDirectory, "/.crawler");
    // open and check if its valid
    FILE* file = fopen(crawler, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Unable to open/access .crawler file for %s directory", pageDirectory);
        return false;
    }
    // close, free the path, return true
    fclose(file);
    mem_free(crawler);
    return true;
}

/***********pagedir_load()***********/
/*
* Takes a crawler file and the docID and turns it into a webpage
* Returns the webpage_t* if it worked and NULL otherwise 
*/
webpage_t* pagedir_load(const char* pageDirectory, const int docID) {
    // hold the size of docID
    int idSize = snprintf(NULL, 0, "%d", docID) + 1;
    // copy the pageDirectory and docID
    char* fileMem = mem_malloc_assert((strlen(pageDirectory) + idSize) * sizeof(char) + 1, "ERROR: Out of memory allocating memory for loading file in pagedir_load\n");
    sprintf(fileMem, "%s/%d", pageDirectory, docID);
    // opens the file to read if it can
    FILE* file = fopen(fileMem, "r");
    mem_free(fileMem);
    if (file == NULL) {
        return NULL;
    }
    // Read the url, depth, and html
    char* url = file_readLine(file);
    char* charDepth = file_readLine(file);
    int depth = sscanf(charDepth, "%d", &depth);
    mem_free(charDepth);
    char* html = file_readUntil(file, NULL);
    // close the file and initialize the webpage
    fclose(file);
    webpage_t* page = webpage_new(url, depth, html);
    if (page == NULL) {
        fprintf(stderr, "ERROR: Out of memory allocating a webpage from a crawler file");
        return NULL;
    }
    return page;
}
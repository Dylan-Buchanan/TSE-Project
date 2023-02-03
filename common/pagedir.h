/*
*  pagedir.h -
*/

#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"

/***********pagedir_init()***********/
/* Constructs the pathname for the .crawler file in that directory
*
*  Opens the file for writing; on error, return false.
*  Closes the file and return true.
*/
bool pagedir_init(const char* pageDirectory);

/***********pagedir_save()***********/
/* Constructs the pathname for the page file in pageDirectory
*  
*  Opens that file for writing, prints the URL, prints the depth,
*  prints the contents of the webpage
*
*  Closes the file
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
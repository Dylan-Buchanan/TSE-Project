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

/***********pagedir_validate()***********/
/*
* Checks to see if a pageDirectory has a .crawler file in it 
* Returns true if it is a valid crawler directory
*/
bool pagedir_validate(const char* pageDirectory);

/***********pagedir_load()***********/
/*
* Takes a crawler file and the docID and turns it into a webpage
* Returns the webpage_t* if it worked and NULL otherwise 
*/
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
/*
* crawler.c - Requires 3 arguments:
* URL, Directory, Depth
* See README.md for instructions about these
* 
* Goes over the url and finds any other url's the page links to, puts them in a bag,
* then inputs information about the page into a file in the directory.
* 
* If the depth of pages has not been reached it then repeats the process over
* the URL's that are in the bag.
*
* Dylan Buchanan, Winter 2023
*/

#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"
#include "mem.h"

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/***********main()***********/
/* Creates varaibles for the arguments which are pointers 
*  to pointers. Then it calls parseArgs() which assigns those
*  pointer pointers if possible. 
*
*  Checks to see if the variables were made successfully
*  Then crawl() is called.
* 
*  Returns 0 if all is successful.
*
*  Should only receive 3 parameters max:
*  seedURL, pageDirectory, maxDepth
*/ 
int main(const int argc, char* argv[]) {
    // intialize the variables
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;
    // parse the arguments
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    // crawl the initial url
    crawl(seedURL, pageDirectory, maxDepth);
    return 0;
}

/***********parseArgs()***********/
/* Given arguments from main, extract them and put them into
*  the functions parameters.
* 
*  Normalize seedURL by passing it into normalizeURL(). It
*  is internal when the URL begins with
*  http://cs50tse.cs.dartmouth.edu/tse/
*  
*  Call pagedir_init() on pageDirectory.
*  If the function returns false the directory is invalid
*
*  Ensure maxDepth is in the specified range
* 
*  Exit 1 if anything goes wrong.
*  Exit 0 if nothing goes wrong.
*/
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {
    // make sure there are only 3 arguments or print an error and exit
    if (argc != 4) {
        fprintf(stderr, "ERROR: please input 3 arguments: seedURL, pageDirectory, maxDepth\n");
        exit(1);
    }
    // normalize the url
    char* norm = normalizeURL(argv[1]);
    if (norm == NULL) {
        fprintf(stderr, "ERROR: invalid seedURL\n");
        exit(1);
    }
    // allocate the memory for the url
    *seedURL = mem_malloc_assert((strlen(norm) * sizeof(char) + 1), "ERROR: out of memory for seedURL\n");
    strcpy(*seedURL, norm);
    // check if it is internal, otherwise print and error and exit
    if (isInternalURL(*seedURL) == false) {
        fprintf(stderr, "ERROR: invalid seedURL\n");
        exit(1);
    }
    mem_free(norm);
    // allocate the memory for the page directory
    *pageDirectory = mem_malloc_assert(strlen(argv[2]) * sizeof(char) + 1, "ERROR: out of memory for pageDirectory\n");
    strcpy(*pageDirectory, argv[2]);
    // create a .crawler file in that directory, if it doesn't work then exit
    if (pagedir_init(*pageDirectory) == false) {
        fprintf(stderr, "ERROR: invalid pageDirectory\n");
        exit(1);
    }
    // get the depth from arguments
    int dep;
    if (sscanf(argv[3], "%d", &dep) != 1) {
        fprintf(stderr, "ERROR: maxDepth was not a valid integer\n");
        exit(1);
    }
    // assign the depth to memory
    *maxDepth = dep;
    // ensure it is a number >= 0 and <= 10
    if (dep < 0 || dep > 10) {
        fprintf(stderr, "ERROR: maxDepth out of range (0-10)\n");
        exit(1);
    }
}

/***********crawl()************/
/* Initializes the hashtable and adds the seedURL
*  Initializes the bag and adds a webpage representing the seedURL at depth 0
*
*  While bag is not empty:
*  Pulls a webpage from the bag, fetches the HTML for that webpage (webpage_fetch())
*  If fetch was successful, saves the webpage to pageDirectory (pagedir_save())
*  If the webpage is not at maxDepth, pageScan() that HTML
*  
*  Then, deletes that webpage, delete the hashtable
*  delete the bag
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
    // initialize the hashtable
    hashtable_t* hash = hashtable_new(200);
    if (hash == NULL) {
        fprintf(stderr, "ERROR: out of memory for hashtable\n");
        exit(1);
    }
    // insert the url to the hashtable to count it as 'visited'
    if (hashtable_insert(hash, seedURL, "") == false) {
        fprintf(stderr, "ERROR: out of memory for hashtable insert\n");
        exit(1);
    }
    // initialize the bag
    bag_t* bag = bag_new();
    if (bag == NULL) {
        fprintf(stderr, "ERROR: out of memory for bag\n");
        exit(1);
    }
    // create the webpage using the url and a NULL html (at depth 0)
    webpage_t* page = webpage_new(seedURL, 0, NULL);
    if (page == NULL) {
        fprintf(stderr, "ERROR: out of memory for webpage creation\n");
        exit(1);
    }
    // the first file created will be '1'
    int docInt = 1;
    // as long as a page exists
    while (page != NULL) {
        // get the html from the page
        if (webpage_fetch(page) == false) {
            fprintf(stderr, "ERROR: could not fetch webpage\n");
            exit(1);
        }
        printf("Fetched: %s\n", webpage_getURL(page));
        // save the page to a file
        pagedir_save(page, pageDirectory, docInt);
        // next file name changed
        docInt++;
        // if we haven't hit max depth yet, scan the page for more URL's
        if (webpage_getDepth(page) < maxDepth) {
            pageScan(page, bag, hash);
        }
        // free the page and get the next page if there is one
        webpage_delete(page);
        page = bag_extract(bag);
    }
    // free the hashtable, bag, and pageDirectory string
    hashtable_delete(hash, NULL);
    bag_delete(bag, NULL);
    mem_free(pageDirectory);
}

/***********pageScan()***********/
/* Given a webpage, scans the given page to extract any links (URLs), 
*  ignoring non-internal URLs. 
*  
*  For any URL not already seen before (i.e., not in the hashtable), 
*  add the URL to both the hashtable pages_seen and to the bag pages_to_crawl.
* 
*  While there is another URL in the page:
*  If that URL is Internal (isInternalURL()) insert the webpage into the hashtable
*  If that succeeded create a webpage_t (webpage_new()) for it and insert the webpage into the bag
*  
*  Free the URL
*/
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
    // start at position '0' on the page
    int pos = 0;
    // get the next url (if there is one)
    char* url = webpage_getNextURL(page, &pos);
    // get the depth of the page for future reference
    int depth = webpage_getDepth(page);
    while(url != NULL) {
        // check if it is internal, if not ignore it
        if (isInternalURL(url) == true) {     
            // check if it has been visited yet, if so ignore it     
            if (hashtable_insert(pagesSeen, url, "") == true) {
                // create a copy of the url so that webpage_delete() can delete its own url
                char* urlCopy = mem_malloc_assert(strlen(url) * sizeof(char) + 1, "ERROR: out of memory for url copy\n");
                strcpy(urlCopy, url);
                // make the next webpage at a depth 1 deeper
                webpage_t* nextpage = webpage_new(urlCopy, depth + 1, NULL);
                if (nextpage == NULL) {
                    fprintf(stderr, "ERROR: out of memory for webpage creation\n");
                    exit(1);
                }
                // insert the page into the bag of pages to crawl
                bag_insert(pagesToCrawl, nextpage);
            }
        }
        // free the previous url
        mem_free(url);
        url = webpage_getNextURL(page, &pos);
    }
}
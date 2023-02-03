/*
*
* crawler.c - 
*
* Dylan Buchanan, Winter 2023
*/

#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "../common/pagedir.h"
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
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
    mem_free(pageDirectory);
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
    if (argc != 4) {
        fprintf(stderr, "ERROR: please input 3 arguments: seedURL, pageDirectory, maxDepth\n");
        exit(1);
    }
    char* norm = normalizeURL(argv[1]);
    *seedURL = mem_malloc_assert((strlen(norm) * sizeof(char) + 1), "ERROR: out of memory for seedURL\n");
    strcpy(*seedURL, norm);
    if (isInternalURL(*seedURL) == false) {
        fprintf(stderr, "ERROR: invalid seedURL\n");
        exit(1);
    }
    mem_free(norm);
    *pageDirectory = mem_malloc_assert(strlen(argv[2]) * sizeof(char) + 1, "ERROR: out of memory for pageDirectory\n");
    strcpy(*pageDirectory, argv[2]);
    if (pagedir_init(*pageDirectory) == false) {
        fprintf(stderr, "ERROR: invalid pageDirectory\n");
        exit(1);
    }
    int dep;
    if (sscanf(argv[3], "%d", &dep) != 1) {
        fprintf(stderr, "ERROR: maxDepth was not a valid integer\n");
        exit(1);
    }
    *maxDepth = dep;
    if (dep < 0 || dep > 10) {
        fprintf(stderr, "ERROR: maxDepth out of range\n");
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
    hashtable_t* hash = hashtable_new(200);
    if (hash == NULL) {
        fprintf(stderr, "ERROR: out of memory for hashtable\n");
        exit(1);
    }
    if (hashtable_insert(hash, seedURL, "") == false) {
        fprintf(stderr, "ERROR: out of memory for hashtable insert\n");
        exit(1);
    }
    bag_t* bag = bag_new();
    if (bag == NULL) {
        fprintf(stderr, "ERROR: out of memory for bag\n");
        exit(1);
    }
    webpage_t* page = webpage_new(seedURL, 0, NULL);
    if (page == NULL) {
        fprintf(stderr, "ERROR: out of memory for webpage creation\n");
        exit(1);
    }
    int docInt = 1;
    while (page != NULL) {
        if (webpage_fetch(page) == false) {
            fprintf(stderr, "ERROR: could not fetch webpage\n");
            exit(1);
        }
        pagedir_save(page, pageDirectory, docInt);
        docInt++;
        if (webpage_getDepth(page) < maxDepth) {
            pageScan(page, bag, hash);
        }
        webpage_delete(page);
        page = bag_extract(bag);
    }
    hashtable_delete(hash, NULL);
    bag_delete(bag, NULL);
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
    int pos = 0;
    char* url = webpage_getNextURL(page, &pos);
    while(url != NULL) {
        if (isInternalURL(url) != false) {          
            if (hashtable_insert(pagesSeen, url, "") != false) {
                char* urlCopy = mem_malloc_assert(strlen(url) * sizeof(char) + 1, "ERROR: out of memory for url copy\n");
                strcpy(urlCopy, url);
                webpage_t* nextpage = webpage_new(urlCopy, webpage_getDepth(page) + 1, NULL);
                if (nextpage == NULL) {
                    fprintf(stderr, "ERROR: out of memory for webpage creation\n");
                    exit(1);
                }
                bag_insert(pagesToCrawl, nextpage);
            }
        }
        mem_free(url);
        url = webpage_getNextURL(page, &pos);
    }
}
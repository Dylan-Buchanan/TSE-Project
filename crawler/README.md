# CS50 Lab 4
## CS50 Winter 2023

### crawler

A `crawler` is a program that handles finding URLs in a website within the _tiny-search-engine_ for CS50. It takes 3 arguments: _seedURL_ (The URL to start at), _pageDirectory_ (The directory to put output files into), and _maxDepth_ (How many webpages deep for the crawler to go from the seedURL. This does not mean the total number of URLs to crawl). 

### Usage

The *crawler* module, implemented in `crawler.c`, takes the 3 arguments stated above and uses the following functions:

```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
```

### Implementation

The `main` function simply initializes variables for the _seedURL_, _pageDirectory_, and _maxDepth_ arguments. It then calls the `parseArgs` function to check the arguments' validity. It then calls the `crawl` function using the parsed variables and returns _0_.

The `parseArgs` function checks to see if only 3 arguments were given. Then it normalizes the URL and ensures that it begins with _http://cs50tse.cs.dartmouth.edu/tse/_ meaning it is an internal URL. It stores the _seedURL_ to memory if it is acceptable. Then it tries to open _pageDirectory_. If it succeeds it stores the directory to memory. Then it checks to see if _maxDepth_ is >= 0 and <= 10. If it is then the depth is stored to memory.

The `crawl` function creates a _hashtable_ that stores all the URLs _crawler_ has visited, and a _bag_ that stores the pages _crawler_ needs to go over. If the website has a valid _HTML_ then it saves the page to a new file and calls `pageScan` and repeats this process until bag is empty.

The `pageScan` function scans a _webpage_ for URLs and checks if the URL is valid and has not been visited before. If so then a new _webpage_ is created and stored in bag. This continues until there are no more URLs on the webpage.

### Assumptions

We assume that the _pageDirectory_ has no files named 1, 2, 3,... inside of it.

We assume that URLs that are not normalized as stated above are not to be checked.

### Files

* `Makefile` - compilation procedure
* `webpage.h` - the interface for webpage handling
* `webpage.c` - the implementation of webpage handling
* `libcs50-given.a` - the archive for the bag.h, bag.c, hashtable.h, hashtable.c, mem.h, and mem.c 
* `pagedir.h` - the interface for file creation and directory validation
* `pagedire.c` - the implementation of file creation and directory validation
* `testing.sh` - unit test driver
* `testing.out` - output of test driver
* `tse-output/` - folder for files to be input to

### Compilation

To compile, simply `make crawler`.

### Testing

The `testing.sh` script tests possible edge cases and errors putting results into testing.out

To test with valgrind, `make valgrind`.
Note: valgrind requires user input if you wish to change which website is being searched and at what depth.
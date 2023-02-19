# CS50 Lab 6
## CS50 Winter 2023

```
****************
* EXTENSION USED
****************
```
```
****************
* EXTENSION USED
****************
```

### querier

`querier.c` used to search for certain words from a _query_ using an _index file_ and a _crawler_ directory.

`testing.sh` tests the modules created by _querier.c_

### querier

The *querier* module asks the user for input. It then parses the input and searches through the index file given to it for those key words. It prints out the results in order from the highest number of appearances to lowest. The *querier* module takes two parameters: a _crawler_ directory and an _indexer_ file.

```c
static query_t* parseInput(char* input);
static bool parseAndOr(query_t* query);
static void askInput(index_t* index, const char* pageDirectory);
static int minimum(int a, int b);
static void andHelper(void* arg, const int docID, const int count);
static counters_t* mergeAnd(counters_t* ct1, counters_t* ct2);
static void orHelper(void* arg, const int docID, const int count);
static counters_t* mergeOr(counters_t* ct1, counters_t* ct2);
static counters_t* findQuery(index_t* index, const char* pageDirectory, query_t* query);
static void resultHelper(void* arg, const int docID, const int count);
static void resultPrint(counters_t* ct, const char* pageDirectory);
```

`parseInput` Takes a char* from the parameters and ensures it is valid and creates and returns a query structure that holds the words

`parseAndOr` Makes sure that and/or are used properly and that no 2 letter words are left

`askInput` Asks the user for input and reads one line at a time. Calls findQuery which looks up the user's words in the index

`minimum` gives the minimum of two ints

`andHelper` Called while a counters object is being iterated through to make a new counters object that has "and" rules applied

`mergeAnd` Merges 2 counters objects with the "and" rule and calls the helper function when iterating

`orHeleper` A helper function that looks to combine two counters objects together

`mergeOr` Combines 2 counters objects using the "or" rule and calls a helper function when iterating

`findQuery` Creates a counters object from the query asked

`resultHelper` A helper function for iteration of a counters object in printing ordered results

`resultPrint` Prints the results of the query in order from highest score to lowest score
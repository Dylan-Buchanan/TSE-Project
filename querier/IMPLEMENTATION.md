# CS50 TSE Querier
## Implementation Spec

### Data structures

We use three data structures: 1. _query_ which holds the words from the query as well as the number of words in the query 2. _mergeSet_ which holds the new _counters_ object that is being created and the _counters_ object that is being compared to for merging 3. _max_ which holds the docID that has the highest score in a _counters_ object and the _counters_ object itself.

### Control flow

The Querier is implemented in one file querier.c, with four main functions and six accessory functions to streamline code

### main

The main function parses the command line, validates parameters, and initializes the index from the indexer file. It calls askInput with pageDirectory and the index. It takes a crawler directory and the name of a corresponding _indexer_ file.

### askInput

Asks the user for input and reads one line at a time parsing it for problems. Calls findQuery which looks up the user's words in the _indexer_ file.

Psuedocode:
ask for user input
parse the input
    handle spaces
    convert to lowercase
    ensure proper syntax
    print the clean query for the user to see
call findQuery
call resultPrint

*parseInput* - A helper function which checks for any invalid characters or words that are too small. It also ensures the query is lowercase

Pseudocode:
Loop through the query characters
    if it isn't a letter or space print error
Loop through again
    after finding a character
    at the next space put a null character
    normalize the word
    put the word in an array
return the list of words

*parseAndOr* - Another helper function which ensures and's and or's are in the correct spot (not next to eachother or at the front or end)

Pseudocode:
Loop through the array of words
    if the first or last word is and/or print error
    if the previous and current word is and/or print error

### findQuery

Creates a counters object from the query asked with all the document ID's matching the query. The outer loop can assume that the next word is not and/or do to the structure.

Pseudocode:
initialize a result counter
for each word
    intialize an inner counter
    for each word
        if the word is 'or' break
        merge the counters for the current word and the inner counter
    update the place that the inner loop got to for the outer loop
    merge the result counter and the inner counter
return the result counter

*mergeAnd* - Adds the intersections of two counters to a new counter. If a docID is in both then the lower count is added.

Pseudocode:
Create a structure to hold the new counter and the second counter
Iterate through the first counter
    Add intersections to the new counter
Remove the first counter as it is no longer needed

*mergeOr* - Adds all unique docIDs of two counters to a new counter. If a docID is in both then the counts of both are added.

Pseudocode:
Create a structure to hold the new counter and the second counter
Iterate through the second counter
    Add everything into the new counter
If there is a first counter to check
Iterate throught the first counter
    Add everything that isn't in the new counter
    If it already is in the new counter add the counts to eachother
Remove the first counter if possible


### resultPrint

Prints the results of the query in order from highest score to lowest score

Pseudocode:
Find a max count in the result counter
If a max count can be found in a counter
    find the max count and get its docID
    Get the URL from the docID
    Print the docID information
    recursively call itself


## Other Modules
### query

Adds three structures: `query_t` holds the query words and number of words, `mergeSet_t` used to hold counters for merging, and `max_t` which is used to hold a _counters_ object and its current max docID.

Adds relating functions to handle using these structures as necessary (creation of structure, deletion, getting values from structure).

*counters_has_doc*
Pseudocode:
Iterate through counter
    If a docID exists set it to arg
return whether a docID was found


### Function Prototypes

querier
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

query
```c
query_t* new_query(char** words, int length);
mergeSet_t* new_merge(counters_t* empty, counters_t* ctB);
max_t* new_max(int* docID, counters_t* ct);
void query_delete(query_t* query);
char** query_get_words(query_t* qeury);
int query_get_length(query_t* query);
counters_t* merge_get_holder(mergeSet_t* merge);
counters_t* merge_get_second(mergeSet_t* merge);
counters_t* max_get_counter(max_t* max);
int* max_get_int(max_t* max);
int* counters_has_doc(counters_t* ct);
int* counters_has_doc(counters_t* ct)
```

`new_query` Creates a new _query_ object.

`new_merge` Creates a new _merge_ object to merge counters.

`new_max` Creates a new _max_ object to find the maximum count in a counter.

`query_delete` Deletes a _query_ object.

`query_get_words` Gets the query of a _query_ object.

`query_get_length` Gets the length of a query in a _query_ object.

`merge_get_holder` Gets the new counter when counters are being merged.

`merge_get_second` Gets the second counter that is being merged.

`max_get_counter` Gets the counter being looked at for the maximum count.

`counters_has_doc` Returns whether or not a _counters_ object contains a document or not.

### Error Handling and Recovery
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem_assert functions), e.g., if a function receives bad parameters.


### Unit Testing
There is only one unit (querier). The querier is the main unit which takes input from a user and searches for the query in an _indexer_ file. Testing will cover all edge cases and error issues. Then it will test on known words with different scenarios.
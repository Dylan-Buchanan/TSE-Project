/*
* query - Contains useful structures and functions for
* those structures to be used in querier.
*
* Dylan Buchanan, February 2023
*/

/***********Structures***********/
// A strucure to hold the query in querier.c
typedef struct query query_t;

// A structure to hold two sets at the same time for the purposes of merging
typedef struct mergeSet mergeSet_t;

// A structure to hold a counter and the current max count docID for sorting
typedef struct max max_t;

/***********new_Query()***********/
/*
* Creates a new query object
* If no query was based in then return NULL
* Otherwise create a query object and return it
*/
query_t* new_query(char** words, int length);


/***********new_merge()***********/
/*
* Creates a new merge object
* It doesn't matter if the counters exist or not,
* This is just a holder of counters
*/
mergeSet_t* new_merge(counters_t* empty, counters_t* ctB);

/***********new_query()***********/
/*
* Creates a new max counter object
* Holds the max docID and and the counter being looked through
*/
max_t* new_max(int* docID, counters_t* ct);

/**********query_delete()***********/
/*
* Deletes a query object and the query passed into it
*/
void query_delete(query_t* query);

/***********query_get_words()***********/
/*
* Returns the query from a query object
*/
char** query_get_words(query_t* qeury);

/***********query_get_length()***********/
/*
* Returns the length from a query object
*/
int query_get_length(query_t* query);

/***********merge_get_holder()***********/
/*
* Returns the merge holder counter
*/
counters_t* merge_get_holder(mergeSet_t* merge);

/***********merge_get_second()***********/
/*
* Returns the second counter being merged
*/
counters_t* merge_get_second(mergeSet_t* merge);

/***********max_get_counter()***********/
/*
* Returns the counter for the max object;
*/
counters_t* max_get_counter(max_t* max);

/***********max_get_int()***********/
/*
* Returns the int* for the docID in the max object
*/
int* max_get_int(max_t* max);
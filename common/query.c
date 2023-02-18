/*
* query - Contains useful structures and functions for
* those structures to be used in querier.
* 
* Dylan Buchanan, February 2023
*/

#include "../libcs50/mem.h"
#include "../libcs50/counters.h"

/***********Structures***********/
// A strucure to hold the query in querier.c
typedef struct query {
    char** words;
    int length;
} query_t;

// A structure to hold two sets at the same time for the purposes of merging
typedef struct mergeSet {
    counters_t* merge;
    counters_t* ct2;
} mergeSet_t;


// A structure to hold a counter and the current max count docID for sorting
typedef struct max {
    int* docID;
    counters_t* ct;
} max_t;

/***********new_query()***********/
// see query.h for function information
query_t* new_query(char** words, int length) {
    // if there is a query to pass
    if (words == NULL) {
        return NULL;
    }
    // assign memory and set variables
    query_t* query = mem_malloc_assert(sizeof(query_t), "ERROR: Out of memory allocating for query structure\n");
    query->words = words;
    query->length = length;
    return query;
}

/***********new_merge()***********/
// see query.h for function information
mergeSet_t* new_merge(counters_t* empty, counters_t* ctB) {
    // assign memory and set variables
    mergeSet_t* merge = mem_malloc_assert(sizeof(mergeSet_t), "ERROR: Out of memory allocating merge set\n");
    merge->merge = empty;
    merge->ct2 = ctB;
    return merge;
}

/***********new_max()***********/
// see query.h for function information
max_t* new_max(int* docID, counters_t* ct) {
    // assign memory and set variables
    max_t* max = mem_malloc_assert(sizeof(max_t), "ERROR: Out of memory allocating max structure\n");
    max->docID = docID;
    max->ct = ct;
    return max;
}

/***********query_delete()***********/
// see query.h for function information
void query_delete(query_t* query) {
    // loop through the words and free them
    for (int i = 0; i < query->length; i++) {
        mem_free(query->words[i]);
    }
    // free everything else
    mem_free(query->words);
    mem_free(query);
}

/***********query_get_words()***********/
// see query.h for function information
char** query_get_words(query_t* qeury) {
    return qeury->words;
}

/***********query_get_length()***********/
// see query.h for function information
int query_get_length(query_t* query) {
    return query->length;
}

/***********merge_get_holder()***********/
// see query.h for function information
counters_t* merge_get_holder(mergeSet_t* merge) {
    return merge->merge;
}

/***********merge_get_second()***********/
// see query.h for function information
counters_t* merge_get_second(mergeSet_t* merge) {
    return merge->ct2;
}
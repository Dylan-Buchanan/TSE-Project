/*
* query - Contains useful structures and functions for
* those structures to be used in querier.
* 
* Dylan Buchanan, February 2023
*/

#include "../libcs50/mem.h"
#include "../libcs50/counters.h"

static void has_doc_helper(void* arg, const int docID, const int count);

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
char** query_get_words(query_t* query) {
    return query->words;
}

/***********query_get_length()***********/
// see query.h for function information
int query_get_length(query_t* query) {
    return query->length;
}

/***********merge_get_holder()***********/
// see query.h for function information
counters_t* merge_get_holder(mergeSet_t* merge) {
    if (merge->merge == NULL) {
        return NULL;
    }
    return merge->merge;
}

/***********merge_get_second()***********/
// see query.h for function information
counters_t* merge_get_second(mergeSet_t* merge) {
    if (merge->ct2 == NULL) {
        return NULL;
    }
    return merge->ct2;
}

/***********max_get_counter()***********/
// see query.h for function information
counters_t* max_get_counter(max_t* max) {
    if (max->ct == NULL) {
        return NULL;
    }
    return max->ct;
}

/***********max_get_int()***********/
// see query.h for function information
int* max_get_int(max_t* max) {
    return max->docID;
}

/***********has_doc_helper()***********/
/*
* Sets the docID being looked at to the docID being passed in as an arg
*/
static void has_doc_helper(void* arg, const int docID, const int count) {
    int* doc = arg;
    *doc = docID;
}

/***********has_doc_helper()***********/
// see query.h for function information
int* counters_has_doc(counters_t* ct) {
    int* docID = mem_malloc_assert(sizeof(int), "ERROR: Out of memory allocating doc\n");
    *docID = 0;
    counters_iterate(ct, docID, has_doc_helper);
    return docID;
} 
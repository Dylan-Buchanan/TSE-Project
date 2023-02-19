/*
* querier - As long as a valid crawler directory has been made
* and a valid index file has been made, querier takes input from the user
* and tells them how many times certain words were found on what websites.
* These results are ranked from most to least.
*
* Dylan Buchanan, February 2023
*/

#include <string.h>
#include <ctype.h>
#include "../libcs50/file.h"
#include "../libcs50/mem.h"
#include "../libcs50/counters.h"
#include "../libcs50/set.h"
#include "../common/word.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/query.h"

static query_t* parseInput(char* input);
static bool parseAndOr(query_t* query);
static void askInput(index_t* index, const char* pageDirectory);
static int minimum(int a, int b);
static void andHelper(void* arg, const int docID, const int count);
static counters_t* mergeAnd(counters_t* ct1, counters_t* ct2);
static void orHelper(void* arg, const int docID, const int count);
static counters_t* mergeOr(counters_t* ct1, counters_t* ct2);
static counters_t* findQuery();
static void resultHelper(void* arg, const int docID, const int count);
static void resultPrint(counters_t* result, const char* pageDirectory);

/***********main()***********/
/*
* Parses arguments and loads the index from indexFilename
*
* Make sure there are only 2 arguments
* Make sure pageDirectory is a valid crawler directory
* Make sure that indexFilename exists
*
* Ask the user for input
*/
int main(const int argc, char* argv[]) {
    // check that the number of arguments is correct
    if (argc != 3) {
        fprintf(stderr, "Please input 2 arguments in the format: ./querier pageDirectory indexFilename\n");
        exit(1);
    }
    // check to see that the pageDirectory is valid
    char* pageDirectory = mem_malloc_assert(strlen(argv[1]) * sizeof(char) + 1, "ERROR: Out of memory allocating pageDirectory\n");
    strcpy(pageDirectory, argv[1]);
    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "ERROR: %s is not a crawler directory\n", pageDirectory);
        mem_free(pageDirectory);
        exit(1);
    }
    char* indexFilename = mem_malloc_assert(strlen(argv[2]) * sizeof(char) + 1, "ERROR: Out of memory allocating indexFilename\n");
    strcpy(indexFilename, argv[2]);
    // try opening the indexFilename to make sure it exists
    FILE* file = fopen(indexFilename, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: %s is not a valid filename\n", indexFilename);
        mem_free(pageDirectory);
        mem_free(indexFilename);
        exit(1);
    }
    // load the index
    index_t* index = index_load(indexFilename);
    fclose(file);
    mem_free(indexFilename);
    // ask the user for input
    askInput(index, pageDirectory);
    // clean up
    mem_free(pageDirectory);
    index_delete(index);
}

/***********parseInput()***********/
/*
* Takes a char* from the parameters and ensures it is valid
* and creates and returns a query structure that holds the words
*
* Make sure there are non letter characters
* Create a list of char*
* Track the front of the word
* Once the back of the word is reached make the word lowercase and add it to the list
* Create the query structure
*
* Return the query struct when successful
* Return NULL when criteria aren't met
*/
static query_t* parseInput(char* input) {
    printf("\n");
    // loop through the input
    for (int a = 0; a < strlen(input); a++) {
        // if it is a space or letter ignore it
        if (isspace(input[a]) || isalpha(input[a])) {
            continue;
        }
        // otherwise the input is invalid
        fprintf(stdout, "ERROR: Bad character '%c' in query.\n", input[a]);
        return NULL;
    }
    // initialize the list for the maximum possible number of words based on the input length
    char** words = mem_malloc_assert(((int) strlen(input) / 3) * sizeof(char*), "ERROR: Out of memory allocating list of words from input");
    // intialize placeholders
    int front;
    int wordNum = 0;
    bool inWord = false;
    int length = strlen(input) + 1;
    // loop through each character of the input
    for (int i = 0; i < length; i++) {
        // if it is a letter
        if (isalpha(input[i])) {
            // and we aren't in the middle of a word
            if (!inWord) {
                // set where the front of this new word is
                inWord = true;
                front = i;
                continue;
            }
            // otherwise ignore it
            continue;
        }
        // if we are in the middle of a word and it isn't a letter
        if (inWord) {
            // set the character to null indicating the end of a word
            input[i] = '\0';
            // make the word
            char* newWord = mem_malloc_assert((i - front) * sizeof(char) + 1, "ERROR: Out of memory allocating new word\n");
            for (int ind = 0; ind <= i - front; ind++) {
                newWord[ind] = input[ind + front];
            }
            // normalize it and put it in the list
            words[wordNum] = normalizeQuery(newWord);
            // check that it worked (only fails if too big)
            if (words[wordNum] == NULL) {
                fprintf(stdout, "ERROR: %s is not a big enough word.\n", newWord);
                mem_free(newWord);
                for (int a = 0; a < wordNum; a++) {
                    mem_free(words[a]);
                }
                mem_free(words);
                return NULL;
            }
            // increment how many words were successful
            wordNum++;
            inWord = false;
            mem_free(newWord);
        }
    }
    // create the query using the list and number of words and return it
    query_t* query = new_query(words, wordNum);
    return query;
}

/***********parseAndOr()***********/
/*
* Makes sure that and/or are used properly and that no
* 2 letter words are left
*
* Returns true if the query is correctly formatted
* Returns false otherwise
*/
static bool parseAndOr(query_t* query) {
    // get the words and length
    char** words = query_get_words(query);
    int length = query_get_length(query);
    // check that and/or isn't used at the front or back
    if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0) {
        fprintf(stdout, "ERROR: Cannot begin query with '%s'.\n", words[0]);
        return false;
    }
    if (strcmp(words[length - 1], "and") == 0 || strcmp(words[length -1], "or") == 0) {
        fprintf(stdout, "ERROR: Cannot end query with '%s'.\n", words[length - 1]);
        return false;
    }
    // keep track if the previous word was and/or
    bool prev = false;
    for (int i = 0; i < length; i++) {
        // if it is and/or
        if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) {
            // and the previous one was too, return false
            if (prev) {
                fprintf(stdout, "ERROR: Cannot put '%s' next to 'and' / 'or'.\n", words[i]);
                return false;
            }
            // otherwise update that there was an and/or
            prev = true;
        }
        // ensure that there are no 2 letter words left
        else {
            if (strlen(words[i]) < 3) {
                fprintf(stdout, "ERROR: %s is not a big enough word.\n", words[i]);
                return false;
            }
            // otherwise ensure reset
            prev = false;
        }
    }
    return true;
}

/***********askInput()***********/
/*
* Asks the user for input and reads one line at a time.
* Calls findQuery which looks up the user's words in the index
*
* Let the user input words
* Parse the input
* Call findQuery() to find the websites that have those words
* Clean up the query
*/
static void askInput(index_t* index, const char* pageDirectory) {
    // get input from user
    fprintf(stdout, "\nQuery? ");
    char* input = file_readLine(stdin);
    // if they stopped input then finish asking
    if (input == NULL) {
        fprintf(stdout, "\n");
        return;
    }
    // parse it
    query_t* query = parseInput(input);
    // if the query has valid characters
    if (query != NULL) {
        // check if the query as valid syntax
        if (parseAndOr(query)) {
            // print the query as it is valid
            query_print(query);
            // look up the words
            counters_t* result = findQuery(index, pageDirectory, query);
            // check if any docIDs were found
            int* hasDoc = counters_has_doc(result);
            if (result == NULL || *hasDoc == 0) {
                fprintf(stdout, "No documents match.\n");
            }
            else {
                resultPrint(result, pageDirectory);
            }
            // free the results
            counters_delete(result);
            mem_free(hasDoc);
        }
        // clean
        query_delete(query);
    }
    // clean and ask the user again for another query
    mem_free(input);
    askInput(index, pageDirectory);
}

/***********minimum()***********/
/*
* If a > b it returns b
* If b > a it returns a
* If a = b it returns a
*/
static int minimum(int a, int b) {
    return (a > b) ? b : a;
}

/***********andHelper()***********/
/*
* Called while a counters object is being iterated through to
* make a new counters object that has "and" rules applied
*
* If both counters being compared have the same docID
* then the new counters object has that docID put
* into it with the count of the lowest counters object
* for that docID
*/
static void andHelper(void* arg, const int docID, const int count) {
    // grab the counters from the argument
    mergeSet_t* merge = arg;
    counters_t* empty = merge_get_holder(merge);
    counters_t* ct2 = merge_get_second(merge);
    // if there is no second counter that means there is no intersection of docIDs
    if (ct2 == NULL) {
        return;
    }
    // otherwise get the count from the second counter
    int count2 = counters_get(ct2, docID);
    // if there is a value set the new holder counters object with the minimum value
    if (count2 != 0) {
        counters_set(empty, docID, minimum(count, count2));
    }
}

/***********mergeAnd()***********/
/*
* Merges 2 counters objects with the "and" rule
* Calls the helper function when iterating
*/
static counters_t* mergeAnd(counters_t* ct1, counters_t* ct2) {
    // create a new counters object
    counters_t* merged = counters_new();
    if (merged == NULL) {
        fprintf(stderr, "ERROR: Out of memory allocating counter for query words.\n");
        return NULL;
    }
    // create the structure to hold multiple counters at the same time
    mergeSet_t* mergeHolder = new_merge(merged, ct2);
    // iterate through the first counters object
    counters_iterate(ct1, mergeHolder, andHelper);
    // delete it since a new one will be made
    counters_delete(ct1);
    mem_free(mergeHolder);
    return merged;
}

/***********orHelper()***********/
/*
* A helper function that looks to combine two counters objects
* together.
* Counts are added together
*/
static void orHelper(void* arg, const int docID, const int count) {
    // grab the counters from the argument
    mergeSet_t* merge = arg;
    counters_t* empty = merge_get_holder(merge);
    counters_t* ct2 = merge_get_second(merge);
    // if the docID being looked at doesn't already exist in the new counters object
    if (counters_get(empty, docID) == 0) {
        // add it
        counters_set(empty, docID, count);
        return;
    }
    // otherwise get the count from the second counters object
    int count2 = counters_get(ct2, docID);
    // if it exists then add the counts in the new counters object
    if (count2 != 0) {
        counters_set(empty, docID, count + count2);
    }
}

/***********mergeOr()***********/
/*
* Combines 2 counters objects using the "or" rule
* Calls a helper function when iterating
*/
static counters_t* mergeOr(counters_t* ct1, counters_t* ct2) {
    // create a new counters object
    counters_t* merged = counters_new();
    if (merged == NULL) {
        fprintf(stderr, "ERROR: Out of memory allocating counter for query words.\n");
        return NULL;
    }
    // add the counters to a single structure for easy access
    mergeSet_t* mergeHolder = new_merge(merged, ct2);
    // loop through all the docIDs
    counters_iterate(ct2, mergeHolder, orHelper);
    // if the first counters object exist make sure its IDs are in the new counters object as well
    if (ct1 != NULL) {
        counters_iterate(ct1, mergeHolder, orHelper);
        counters_delete(ct1);
        counters_delete(ct2);
    }
    mem_free(mergeHolder);
    return merged;
}

/***********findQuery()***********/
/*
* Creates a counters object from the query asked
* 
* Has an outer loop that calls an inner loop at
* the start of a query and after every 'or'
* in a query. This allows the inner loop to
* only have to concern itself with 'and' keywords
*/
static counters_t* findQuery(index_t* index, const char* pageDirectory, query_t* query) {
    // grab query and create an unitialized set holder
    char** words = query_get_words(query);
    int length = query_get_length(query);
    counters_t* result = NULL;
    // go through each word in the query
    for (int i = 0; i < length; i++) {
        // intialize a counters object for this section of the query
        counters_t* prevCt = NULL;
        int a;
        for (a = i; a < length; a++) {
            // if the word is 'or' break
            if (strcmp(words[a], "or") == 0) {
                break;
            }
            // if the word is 'and' continue
            if (strcmp(words[a], "and") == 0) {
                continue;
            }
            // if this isn't the first word 
            if (prevCt != NULL) {
                // then the only reason to still be in the loop is an and was called
                prevCt = mergeAnd(prevCt, index_find(index, words[a]));
            }
            // otherwise
            else {
                // if this isn't the first word move on
                if (a != i) {
                    continue;
                }
                // if the current word doesn't exist in the website move on
                if (index_find(index, words[a]) == NULL) {
                    continue;
                }
                // otherwise add the word's counts to the running counters object
                prevCt = mergeOr(prevCt, index_find(index, words[a]));
            }
        }
        // get the outer loop caught up
        i = a;
        // if the result has nothing yet then set it to what was from the inner loop
        if (result == NULL) {
            result = prevCt;
            continue;
        }
        // otherwise combine inner and outer because to be here an 'or' had to be called
        if (prevCt != NULL) {
            result = mergeOr(result, prevCt);
        }
    }
    return result;
}

/***********resultHelper()***********/
/*
* A helper function for iteration of a counters object
* 
* Sets the docID of the argument to the greatest count
* for a docID
*/
static void resultHelper(void* arg, const int docID, const int count) {
    // get the counters object and max docID from the arguments
    max_t* max = arg;
    counters_t* ct = max_get_counter(max);
    int* id = max_get_int(max);
    // set the docID to whichever has a greater count
    if (counters_get(ct, *id) < count) {
        *id = docID;
    }
}

/***********resultPrint()***********/
/*
* Prints the results of the query in order from highest score
* to lowest score
*
* Passes an int* into a counters object iteration to find the
* docID with the highest score
* Prints out the highest docID information then recursively
* calls itself until all docIDs have a count of 0
*/
static void resultPrint(counters_t* ct, const char* pageDirectory) {
    // create an int*
    int* docID = mem_malloc_assert(sizeof(int), "ERROR: Out of memory for allocating docID\n");
    *docID = 0;
    max_t* max = new_max(docID, ct);
    // iterate through the counter to find the max count of a docID
    counters_iterate(ct, max, resultHelper);
    mem_free(max);
    if (*docID != 0) {
        // create the strings to print
        int idSize = snprintf(NULL, 0, "%d", *docID) + 1;
        char* doc = mem_malloc_assert((strlen(pageDirectory) + idSize) * sizeof(char) + 1, "ERROR: Out of memory allocating for document path\n");
        sprintf(doc, "%s/%d", pageDirectory, *docID);
        // get the URL from the corresponding crawler file of the max docID
        FILE* file = fopen(doc, "r");
        char* html = file_readLine(file);
        fclose(file);
        // print out the max docID information
        fprintf(stdout, "Score:  %d || Doc:  %d || %s\n", counters_get(ct, *docID), *docID, html);
        // set that docID to 0 so it won't come up again
        counters_set(ct, *docID, 0);
        // clean up
        mem_free(doc);
        mem_free(html);
        // look for the next max
        resultPrint(ct, pageDirectory);
    }
    mem_free(docID);
}
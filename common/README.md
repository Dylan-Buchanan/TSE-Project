# CS50 TSE Lab
## CS50 Winter 2023

### common/

Contains useful files for TSE lab.

`pagedir.c` and `pagedir.h` are used in the *crawler* and *indexer* module.

`index.c` and `index.h` are used in the *indexer* module.

`word.c` and `word.h` are used in the *indexer* module.

### pagedir

Contains helpful functions for handling directories

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
```

`pagedir_init` constructs the pathname for the .crawler file in that directory. It then opens the file for writing based on that pathname. On error it returns false. Otherwise it closes the file and return true.

`pagedir_save` constructs the pathname for the page file in pageDirectory. It then opens that file for writing, prints the URL, prints the depth, and prints the contents of the webpage into a file.

`pagedir_validate` checks to see if a pageDirectory has a .crawler file in it. It will return true if it is a valid crawler directory and otherwise return false.

`pagedir_load` takes a crawler file and the docID of it and turns it into a webpage. It returns a `webpage_t*` object if it worked and _NULL_ otherwise. 

### index

Contains a new structure `index_t` that acts as a wrapper of a _hashtable_. It stores a word as a _key_ and stores a `counter_t*` object as an item. The counter contains all the documents that the word is seen in and the number of times it is seen in that document.

```c
index_t* index_new(int size);
counters_t* index_find(index_t* index, char* key);
void index_update(index_t* index, const char* key, const int docID);
void index_save(index_t* index, const char* indexFilename);
void index_delete(index_t* index);
void index_set(index_t* index, char* key, counters_t* ct);
index_t* index_load(const char* oldIndexFilename);
```

`index_new` creates a new _index_ object from an input of the max number of bins for the hashtable. It creates an empty hashtable.

`index_find` finds the counter for the specified key and returns it. It takes an _index_ object and a _key_ as arguments.

`index_update` updates the count that a _key_ appears for a specific document by 1. It takes an _index_ object, a _key_, and the _docID_ that the _key_ is to be updated for as arguments.

`index_save` creates an index file in the format: word [docID count] [docID count] ... [] (brackets are not included). It takes an _index_ object and an filename for where to write the information to.

`index_delete` deletes the hashtable in index and frees the index. It takes an _index_ object as an argument.

`index_set` inputs a specific _counter_ into an index for a specific _key_ so that manual insertion is possible. Takes a _counter_ object and _key_ as arguments.

`index_load` takes an index filename, reads it, and turns it into an index object that is returned. It splits each line into parts: word, docID/count pairs. It takes the name of the file as an argument.

### word

Used to make sure a word is longer than 2 letters, has no numbers, and makes it lowercase.

```c
char* normalizeWord(const char* word);
```

`normalizeWord` takes a _char*_ as an argument and normalizes it as stated above.

### query

Contains many useful structures and functions for querier.c.

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
# CS50 Lab 5
## CS50 Winter 2023

### Data structures
We use one data structure: an 'index' of words that point to the number of times they appear in certain documents.

### Control flow
The Indexer is implemented in one file indexer.c, with three functions.

### main
The main function parses the command line, validates parameters, and initializes other modules. It calls indexBuild with pageDirectory. It takes a crawler directory and the name of the desired output for the index file.

### indexBuild
Creates the index file based on a crawler directory and uses indexPage to add information to the _index_ object that it saves.

Pseudocode:
intialize the index
start at doc 1
load a webpage for that id
while a webpage can be loaded
    call indexPage
    delete the webpage and get the next one
save the index to the file and delete the index

### indexPage
Creates the index file based on a crawler directory.

Pseudocode:
get all the words on the page
while you can get a word
    normalize the word
    if the word normalized properly, increment its occurrence
    free the words
    get the next word
clean up

## Other modules
### indextest
Takes a index file and converts it to an `index_t` object. Then turns the index back into a file.

Pseudocode:
check correct number of arguments
grab names from arguments
load the index
check that it loaded properly
save the index
delete the index and free the filenames

### index
Contains a new structure `index_t` that acts as a wrapper of a _hashtable_. It stores a word as a _key_ and stores a `counter_t*` object as an item. The counter contains all the documents that the word is seen in and the number of times it is seen in that document.

Pseudocode:
index_new:
create a hashtable

index_find:
check to see if the index and key are valid
return the counter for the key if it exists

index_update:
check to see if the arguments are valid
find the counter if it exists
    if it doesn't make a new one
    add a new document to the counter since its new
    insert the counter into the hashtable
otherwise simply increment the count for that document

index_save:
check if the parameters are valid
iterate through the hashtable of the index
    print the word
    iterate through the counter
        print the doc and count

index_delete:
check if the index is valid
free the hashtable
    free the counter
free the index

index_set:
check if the parameters are valid
insert the word and counters object into the hashtable

index_load:
open an index file
while there is a line to parse
    split the line
    store the word
    while there are docID count pairs
        store in a counter
    put word and counter into index
return index


### word
Used to make sure a word is longer than 2 letters, has no numbers, and makes it lowercase.

Pseudocode:
Check for correct length of word
Loop through characters
    check that character is a letter
    ensure the letter is lowercase
return lower case word

### pagedir
Add two functions: `pagedir_validate` checks to see if a pageDirectory has a .crawler file in it. It will return true if it is a valid crawler directory and otherwise return false; `pagedir_load` takes a crawler file and the docID of it and turns it into a webpage. It returns a `webpage_t*` object if it worked and _NULL_ otherwise. 

Pseudocode:
pagedir_validate:
check that a directory has a .crawler file
return true if it does false if not

pagedir_load:
try to open a crawler file
parse the url depth and html
store in a webpage object
return the webpage

### Function prototypes
indexer
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

indextest
```c
int main(const int argc, char* argv[]);
static void indexBuild(const char* pageDirectory, const char* indexFilename);
static void indexPage(index_t* index, webpage_t* page, const int docID);
```

`main` parses the command line, validates parameters, and initializes other modules. It calls indexBuild with pageDirectory. It takes a crawler directory and the name of the desired output for the index file.

`indexBuild` creates the index file based on a crawler directory and uses indexPage to add information to the _index_ object that it saves.

`indexPage` goes through a page a counts the number of times a word appears in a document. It takes an _index_ object, _webpage_ object, and _docID_ as parameters.

index
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

pagedir
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

word
```c
char* normalizeWord(const char* word);
```

`normalizeWord` takes a _char*_ as an argument and normalizes.

### Error handling and recovery
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem_assert functions), e.g., if a function receives bad parameters.

### Unit testing
There are only three units (indexer, indextest, and indexcmp). The indexer is the main unit which creates an index file as stated above. The indextest unit is meant to ensure that loading an index structure from an index file works properly. This is best if coupled with the indexcmp unit which compares the output of indextest and indexer to ensure they are the same
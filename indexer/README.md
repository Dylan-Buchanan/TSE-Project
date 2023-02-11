# CS50 Lab 5
## CS50 Winter 2023

### indexer/

`indexer.c` used to index the output from the *crawler* module

`indextest.c` used to test pulling an _index_ object from an index file

`test.sh` tests the modules created by _indexer.c_ and _indextest.c_

### indexer

Takes a directory from crawler output and the name of the desired index file output and counts the number of times a word appears in each document in the directory storing it in an `index_t` structure. Then it prints the index into the target index file.

```c
int main(const int argc, char* argv[]);
static void indexBuild(const char* pageDirectory, const char* indexFilename);
static void indexPage(index_t* index, webpage_t* page, const int docID);
```

`main` parses the command line, validates parameters, and initializes other modules. It calls indexBuild with pageDirectory. It takes a crawler directory and the name of the desired output for the index file.

`indexBuild` creates the index file based on a crawler directory and uses indexPage to add information to the _index_ object that it saves.

`indexPage` goes through a page a counts the number of times a word appears in a document. It takes an _index_ object, _webpage_ object, and _docID_ as parameters.

### indextest

Takes a index file and converts it to an `index_t` object. Then turns the index back into a file.

```c
int main(const int argc, char* argv[]);
```

`main` does what is stated above.
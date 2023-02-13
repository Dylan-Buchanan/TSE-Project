# CS50 TSE Querier
## Design Spec

According to the Querier Requirement Spec, the TSE _querier_ is a standalone program that reads the input from a user, searches through the document files produced by the TSE indexer, and returns the websites with the most matches for the key words inputted by the user. This document only covers the _querier_.

### User Interface

The querier has two interfaces that the user needs to access.
The first requires two arguments and is on the command-line.
```bash
indexer pageDirectory indexFilename
```
For example, if _letters_ is a pageDirectory in _../tse-output_
```bash
$ indexer ../tse-output/letters ../tse-output/letters.index
```

The second takes any number of arguments and is from _stdin_.
```bash
cat and dog
```

### Inputs and outputs

**Input:** the querier takes words from a user and reads files from a directory as well as information from an _indexer_ file. Keyword 'and' is taken to mean 'find websites with both of these words in them' and keyword 'or' is taken to mean 'find websites with either one word or the other'.

**Output:** the querier tells the user which websites have the most occurences of the keywords that the user searched for.

### Functional decomposition into modules

We anticipate the following modules and functions:

    1. _main,_ parses arguments and initializes data structures
    2. _askInput,_ gets query from _stdin_ and parses the query
    3. _findQuery,_ use the index to identify the set of documents that satisfy the query
    4. _resultPrint,_ prints the documents and scores in proper format

And some helper modules that provide data structures:

    1. _index,_ a module providing the data structure to represent the in-memory index, and functions to read and write index files;
    2. _webpage,_ a module providing the data structure to represent webpages, and to scan a webpage for words;
    3. _pagedir,_ a module providing functions to load webpages from files in the pageDirectory;
    4. _word,_ a module providing a function to normalize a word.

### Pseudo code for logic/algorithmic flow

The querier will run as follows:
```c
parse the command line, validate parameters, initialize index from indexFilename
call askInput with the index and pageDirectory 
```

where _askInput_:
```c
ask for user input
parse the input
    handle spaces
    convert to lowercase
    ensure proper syntax
    print the clean query for the user to see
call findQuery
```

where _findQuery_:
```c
for each word
    look through the built index for words satisfying the and sequence
    store the docIDs and counts of where they are found
while there is an and/or to account for
    take two words applying and/or principles and combine docIDs and counts appropriately
    store new combination
take docIDs found and load webpages
call resultPrint
```

where _resultPrint_:
```c
rank the results based on scores
loop through webpages and scores
    print
```

### Major data structures

The key data structure is the _index_, mapping from word to (_docID_, _#occurrences_) pairs. The _index_ is a _hashtable_ keyed by
_word_ and storing _counters_ as items. The _counters_ is keyed by docID and stores a count of the number of occurrences of that word 
in the document with that ID.

### Testing plan

Integration testing. The querier, as a complete program, will be tested by taking a valid _crawler_ directory and _indexer_ file and using them to search for keywords from user input.

    1. Test querier with various invalid arguments. 2. no arguments 3. one argument 4. three or more arguments 5. invalid pageDirectory
       (non-existent path) 5. invalid pageDirectory (not a crawler directory) 6. invalid indexFile (non-existent path) 7. invalid 
       indexFile (read-only directory) 7. invalid indexFile (existing, read-only file)
    2. Run _querier_ on a variety of pageDirectories with smaller webpages and confirm results.
    3. Run _valgrind_ on _querier_ to ensure no memory leaks or errors.
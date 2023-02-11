#!/bin/bash
#
# A test script for the indexer and indextest binary files
#
# Dylan Buchanan, Februrary 2023

# Invalid number of arguments
./indexer

# Invalid crawler directory
./indexer ../tse-output/doesnotexist ../tse-output/doesnotexist-1-index

# Invalid indexFilename
./indexer ../tse-output/wikipedia-1 /as/f/a/dsd/

# Valid testing with a normal crawler directory
./indexer ../tse-output/wikipedia-1 ../tse-output/wikipedia-1-index

# Test that getting an index from an index file worked
./indextest ../tse-output/wikipedia-1-index ../tse-output/wikipedia-1-inverse

# Ensure the new index file is the same as the old one
./indexcmp ../tse-output/wikipedia-1-index ../tse-output/wikipedia-1-inverse
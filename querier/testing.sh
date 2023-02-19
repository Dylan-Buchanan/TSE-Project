#!/bin/bash
#
# testing - tests various edge cases and uses input from
# a text file to test scenarios from input
#
# Dylan Buchanan, February 2023

# No arguments
./querier

# One argument
./querier ../tse-output/wikipedia-1

# Input from test file
# words - 4:3 3:2 2:1 6:1 7:1
# science - 7:150 4:5 2:4 3:4 1:1 6:1
# birds - No documents
# words and science - 4:3 3:2 2:1 6:1 7:1
# words or science - 7:171 4:8 3:6 2:5 6:2 1:1
# birds and words - No documents
# birds or words - 4:3 3:2 2:1 6:1 7:1
./querier ../tse-output/wikipedia-1 ../tse-output/wikipedia-1-index < testwords.txt
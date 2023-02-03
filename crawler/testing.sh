#!/bin/bash
#
# testing.sh - a testing script for the crawler.c program.
# Requires tse-output/ folder to be created
#
# Dylan Buchanan, January 2023

# Wrong number of arguments
./crawler www.verycool.net ../tse-output 0 hamsandwich

# Invalid URL
./crawler www.verycool.net ../tse-output 0

# Invalid Directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html amazingFolderName 0

# Depth too low, Depth too high
mkdir ../tse-output/wrong-depth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../tse-output/wrong-depth -2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../tse-output/wrong-depth 12

# letters, depth: 0
printf "\nNext Test\n"
mkdir ../tse-output/letters-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../tse-output/letters-0 0

# letter, depth: 10 (depth is greater than number of pages)
printf "\nNext Test\n"
mkdir ../tse-output/letters-10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../tse-output/letters-10 10

# toscrape, depth: 0
printf "\nNext Test\n"
mkdir ../tse-output/toscrape-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../tse-output/toscrape-0 0

# toscrape, depth: 1
printf "\nNext Test\n"
mkdir ../tse-output/toscrape-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../tse-output/toscrape-1 1

# wikipedia, depth: 0
printf "\nNext Test\n"
mkdir ../tse-output/wikipedia-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../tse-output/wikipedia-0 0

# wikipedia, depth: 1
printf "\nNext Test\n"
mkdir ../tse-output/wikipedia-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../tse-output/wikipedia-1 1
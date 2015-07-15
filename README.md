# Twitter
Code Challenge Insight

This file was written in C++ and compiled on a Ubuntu 10.04 OS (Linux 3.2.0-85-generic)
g++-4.6

The routine open the required input file, checks for errors in the file, and then passes
the buffer to a C++ class.
The class scans the buffer and does the following:
1) Populates a tree to sort unique words
2) Looks for unique words in every line (tweet)
3) Computes the number of unique words, updates its frequencygram atfer reading each line
and computes the running median.

The three above are not executed in sequence, i.e. as the buffer is read the tree is populated
with each word and, and the number of unique words per tweet is updated. When the "end of line" 
command is found, the median is updated.

The input file is read with fread, as opposed to process each line one by one, to miniimize
execution time.

#!/usr/bin/env bash

# example of the run script for running the word count

# I'll compile and execute my programs, with the input directory tweet_input and output the files in the directory tweet_output

g++ -o twitter twitter.cpp
./twitter tweet_input/tweets.txt

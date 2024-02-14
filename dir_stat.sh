#!/bin/bash

# Check if user provided a directory
if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

directory=$1

# Check if the directory exists
if [ ! -d "$directory" ]; then
    echo "Error: Directory '$directory' not found"
    exit 1
fi

# Count the number of files and subdirectories
files=$(find "$directory" -type f | wc -l)
directories=$(find "$directory" -mindepth 1 -type d | wc -l)

echo "Total number of files: $files"
echo "Total number of directories: $directories"

# How to run the file 
#--------------------
# chmod 755 dir_stat.sh
# ./ dir_stat.sh /opt

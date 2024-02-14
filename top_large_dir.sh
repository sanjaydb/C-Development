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

# List top 5 largest files in the directory
echo "Top 5 largest files in $directory:"
du -ah "$directory" | sort -rh | head -n 5



# How to execute the script
#--------------------------
# chmod 755 top_large_dir.sh
# ./top_large_dir.sh /opt


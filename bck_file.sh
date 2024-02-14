#!/bin/bash

# Check if user provided a file
if [ $# -ne 1 ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

file=$1
backup_file="$file-$(date +%Y%m%d)"

cp "$file" "$backup_file" && echo "Backup created: $backup_file"


# How to run the file
#---------------------
# chmod 755 bck_file.sh
# ./bck_file.sh source.c 
# output  source.c-20241402

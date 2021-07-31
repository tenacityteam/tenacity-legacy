#!/bin/bash

# Run this script on a directory to handle PO files with no id

shopt -s nullglob

input_dirs="$*"

if [ $# -eq 0 ]; then
    input_dirs[0]="${BASH_SOURCE[0]}/../"
fi

for dir in "${input_dirs[@]}"
do
    for po_file in "${dir}"/*.po; do
       sed -i '/^Project/d' "$po_file"
    done
done

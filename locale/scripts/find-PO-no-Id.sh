#!/bin/sh

# Run this script on a directory to handle PO files with no id
DIRNAME=$(dirname "$0")

if [ $# -eq 0 ]; then
    set -- "$DIRNAME/../"
fi

for dir in "$@"
do
    for po_file in "${dir}"/*.po; do
       sed -i '/^Project/d' "$po_file"
    done
done

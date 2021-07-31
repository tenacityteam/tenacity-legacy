#!/bin/bash

# Run this script on a directory to "degimpify" all XPM files
# within that directory (meaning any file ending with .XPM)
#
# If you run this script with no arguments, it uses the default
# directories, which are the original three directories that were
# being "degimpified" pre-fork.

shopt -s nullglob

input_dirs="$*"

if [ $# -eq 0 ]; then
    input_dirs[0]="./EditButtons"
    input_dirs[1]="./TranscriptionImages"
    input_dirs[2]="./ControlButtons"
fi

for dir in "${input_dirs[@]}"
do
    for xpm_file in "${dir}"/*.xpm; do
      echo "Degimpify: ${xpm_file}"
      sed -e 's/\_xpm\[\]/\[\]/' -e 's/^static char \*/static const char  \*/' -i "$xpm_file"
    done
done

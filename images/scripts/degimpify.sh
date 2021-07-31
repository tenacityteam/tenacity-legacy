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
    input_dirs[0]="${BASH_SOURCE[0]}/../../images/EditButtons"
    input_dirs[1]="${BASH_SOURCE[0]}/../../images/TranscriptionImages"
    input_dirs[2]="${BASH_SOURCE[0]}/../../images/ControlButtons"
fi

for dir in "${input_dirs[@]}"
do
    for xpm_file in "${dir}"/*.xpm; do
      echo "Degimpify: ${xpm_file}"
      sed -e 's/\_xpm\[\]/\[\]/' -e 's/^static char \*/static const char  \*/' -i "$xpm_file"
    done
done

#!/bin/bash

# Report how complete each translation catalog is

# How many messages in total?
total=$(grep -c '^msgid' "../tenacity.pot")


declare -i badlines
for po_file in ../*.po; do
    # If there are errors from msgcmp, then the last line on standard error
    # contains a count of problematic messages; else it won't match the
    # pattern in awk, so assume no errors
    errors=$(msgcmp "$po_file" "../tenacity.pot" 2>&1 | awk '/msgcmp: found [0-9]* fatal error/ { nn = $3 } END {print 0+nn}')
    complete=$((total-errors))

    # detect whether this sequence occurs in any .po file.  It will break msgfmt on Windows.
    badlines=$(grep -F -c '#~|' "${po_file}")

    echo "$po_file: completed $complete of $total ($((complete*100/total))%); Windows Incompatible Lines: $badlines"
done | sort -n -t , -k3

exit 0

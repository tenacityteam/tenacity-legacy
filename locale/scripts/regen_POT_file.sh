#!/bin/bash
# Run this script with /locale/script/ as the current directory
set -o errexit

echo ";; Recreating tenacity.pot using .h, .cpp and .mm files"
for path in "${BASH_SOURCE[0]}/../modules/mod-*" "${BASH_SOURCE[0]}/../libraries/lib-*" "${BASH_SOURCE[0]}/../include" "${BASH_SOURCE[0]}/../src" ; do
   find "$path" -name \*.h -o -name \*.cpp -o -name \*.mm
done | LANG=c sort | \
sed -E 's/\.\.\///g' |\
xargs xgettext \
--no-wrap \
--default-domain=tenacity \
--directory="${BASH_SOURCE[0]}/../.." \
--keyword=_ --keyword=XO --keyword=XC:1,2c --keyword=XXO --keyword=XXC:1,2c --keyword=XP:1,2 --keyword=XPC:1,2,4c \
--add-comments=" i18n" \
--add-location=file  \
--copyright-holder='Tenacity Contributors' \
--package-name="tenacity" \
--package-version='3.0.4' \
--msgid-bugs-address="emabrey@tenacityaudio.org" \
--add-location=file -L C -o "${BASH_SOURCE[0]}/../tenacity.pot"

echo ";; Adding nyquist files to tenacity.pot"
for path in "${BASH_SOURCE[0]}/../../plug-ins/"* ; do find "$path" -name \*.ny -not -name rms.ny; done | LANG=c sort | \
sed -E 's/\.\.\///g' |\
xargs xgettext \
--no-wrap \
--default-domain=tenacity \
--directory="${BASH_SOURCE[0]}/../.." \
--keyword=_ --keyword=_C:1,2c --keyword=ngettext:1,2 --keyword=ngettextc:1,2,4c \
--add-comments=" i18n" \
--add-location=file  \
--copyright-holder='Tenacity Contributors' \
--package-name="tenacity" \
--package-version='3.0.4' \
--msgid-bugs-address="emabrey@tenacityaudio.org" \
--add-location=file -L Lisp -j -o "${BASH_SOURCE[0]}/../tenacity.pot"

echo ""
echo ";; POT file generated"
echo ""

head -n 11 "${BASH_SOURCE[0]}/../tenacity.pot" | tail -n 3
wc -l "${BASH_SOURCE[0]}/../tenacity.pot"

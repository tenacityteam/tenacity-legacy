#!/usr/bin/env bash

((${BASH_VERSION%%.*} >= 4)) || { echo >&2 "$0: Error: Please upgrade Bash."; exit 1; }

set -euxo pipefail

cmd_string=/usr/bin/hdiutil "$@" -debug

max_retry=5

counter=0

until cmd_string
do
   sleep 1
   [[ counter -eq $max_retry ]] && echo "CPack failed despite retry attempts!" && exit 1
   echo "Trying CPack hdiutil call again. Try #$counter"
   ((counter++))
done

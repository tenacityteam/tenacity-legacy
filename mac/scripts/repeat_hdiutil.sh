#!/usr/bin/env bash

((${BASH_VERSION%%.*} >= 4)) || { echo >&2 "$0: Error: Please upgrade Bash."; exit 1; }

set -euxo pipefail

max_retry=5
counter=0
num_secs_await_retry=1

while ! /usr/bin/hdiutil "$@"; do
    sleep $num_secs_await_retry
    ((counter++))
    if [[ $counter -eq $max_retry ]]; then
        echo "CPack failed despite retry attempts!"
        exit 1
    fi
    echo "Trying CPack hdiutil call again. Try #$counter"
done

#!/usr/bin/env bash

((${BASH_VERSION%%.*} >= 4)) || { echo >&2 "$0: Error: Please upgrade Bash."; exit 1; }

set -euxo pipefail

cd build

if [[ "${OSTYPE}" == msys* && ${GIT_BRANCH} == release* ]]; then # Windows
    cmake --build . --target innosetup --config "${AUDACITY_BUILD_TYPE}"
else
   # This will break if you move this script to a different directory
   # because this searches for the current script's directory and the
   # path is relative to that absolute path.
   SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
   cpack -C "${AUDACITY_BUILD_TYPE}" -D CPACK_COMMAND_HDIUTIL="${SCRIPT_DIR}/macos/repeat_hdiutil.sh" --verbose
fi

# Remove the temporary directory
rm -Rf package/_CPack_Packages

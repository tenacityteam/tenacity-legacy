#!/usr/bin/env bash

((${BASH_VERSION%%.*} >= 4)) || { echo >&2 "$0: Error: Please upgrade Bash."; exit 1; }

set -euxo pipefail

cd build

if [[ "${OSTYPE}" == msys* && ${GIT_BRANCH} == release* ]]; then # Windows
    cmake --build . --target innosetup --config "${AUDACITY_BUILD_TYPE}"
else
    cpack -C "${AUDACITY_BUILD_TYPE}" --verbose -DCPACK_COMMAND_HDIUTIL="$(python -c "import os, sys; print(os.path.dirname(os.path.realpath(sys.argv[1])))" "$0")/../../mac/scripts/repeat_hdiutil.sh"
fi

# Remove the temporary directory
rm -Rf package/_CPack_Packages

#!/usr/bin/env bash

((${BASH_VERSION%%.*} >= 4)) || { echo >&2 "$0: Error: Please upgrade Bash."; exit 1; }

set -euxo pipefail

conan --version # check it works

cmake_args=(
    -S .
    -B build
    -G "${AUDACITY_CMAKE_GENERATOR}"
    -D use_pch=no
    -D CMAKE_BUILD_TYPE="${AUDACITY_BUILD_TYPE}"
    -D CMAKE_INSTALL_PREFIX="${AUDACITY_INSTALL_PREFIX}"
)

if [[ -n "${APPLE_CODESIGN_IDENTITY}" && "${OSTYPE}" == darwin* ]]; then
    cmake_args+=(
        -D APPLE_CODESIGN_IDENTITY="${APPLE_CODESIGN_IDENTITY}"
        -D perform_codesign=yes
    )

    if [[ ${GIT_BRANCH} == release* ]]; then
        cmake_args+=(
            -D APPLE_NOTARIZATION_USER_NAME="${APPLE_NOTARIZATION_USER_NAME}"
            -D APPLE_NOTARIZATION_PASSWORD="${APPLE_NOTARIZATION_PASSWORD}"
            -D perform_notarization=yes
        )
    fi
elif [[ -n "${WINDOWS_CERTIFICATE}" && "${OSTYPE}" == msys* ]]; then
    # Windows certificate will be used from the environment
    cmake_args+=(
        -D perform_codesign=yes
    )
fi

if [[ ${GIT_BRANCH} == release* ]]; then
    cmake_args+=(
        -D package_manual=yes
    )
fi

# Configure Audacity
cmake "${cmake_args[@]}"

# Remove build directories and sources to reduce the cache size.
conan remove "*" --src --builds --force

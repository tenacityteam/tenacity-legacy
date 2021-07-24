#!/usr/bin/env bash

((${BASH_VERSION%%.*} >= 4)) || echo >&2 "$0: Warning: Using ancient Bash version ${BASH_VERSION}."

set -euxo pipefail

if [[ "${OSTYPE}" == msys* ]]; then # Windows

    if which choco; then

        # Chocolatey packages
        choco_packages=(
            sccache 
            conan
        )

        choco install "${choco_packages[@]}" -y
    else
        echo >&2 "$0: Error: You don't have a recognized package manager installed."
        exit 1
    fi

elif [[ "${OSTYPE}" == darwin* ]]; then # macOS

    # Homebrew packages
    brew_packages=(
        bash # macOS ships with Bash v3 for licensing reasons so upgrade it now
        conan
        ccache
    )
    brew install "${brew_packages[@]}"

else # Linux & others

    if ! which sudo; then
        function sudo() { "$@"; } # no-op sudo for use in Docker images
    fi

    # Distribution packages
    if which apt-get; then
        apt_packages=(
            # Docker image
            file
            g++
            git
            wget

            # GitHub Actions
            libasound2-dev
            libgtk2.0-dev
            gettext
            ccache
        )
        sudo apt-get update -y
        sudo apt-get install -y --no-install-recommends "${apt_packages[@]}"
        
        # Download Conan from github
        wget "https://github.com/conan-io/conan/releases/latest/download/conan-ubuntu-64.deb" -nv -O /tmp/conan.deb
        sudo dpkg -i /tmp/conan.deb
    else
        echo >&2 "$0: Error: You don't have a recognized package manager installed."
        exit 1
    fi

fi

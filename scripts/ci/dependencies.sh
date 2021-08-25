#!/usr/bin/env bash

((${BASH_VERSION%%.*} >= 4)) || echo >&2 "$0: Warning: Using ancient Bash version ${BASH_VERSION}."

set -euxo pipefail

if [[ "${OSTYPE}" == msys* ]]; then # Windows

    if which choco; then

        # Chocolatey packages
        choco_packages=(
            sccache
        )

        # This retry workaround is because on Github actions this just randomly fails when
        # the CI server isn't able to connect to the choco server for some reason...

        set +ex

        max_retry=5
        counter=0
        num_secs_await_retry=5

        until choco install "${choco_packages[@]}" -y; do
           sleep $num_secs_await_retry
           if [[ $counter -eq $max_retry ]]; then
                echo "choco install failed despite retry attempts"
                exit 1
           else
                echo "Trying choco install again..."
                ((counter++))
           fi
        done

       set -ex
    else
        echo >&2 "$0: Error: You don't have a recognized package manager installed."
        exit 1
    fi

elif [[ "${OSTYPE}" == darwin* ]]; then # macOS

    # Homebrew packages
    brew_packages=(
        bash # macOS ships with Bash v3 for licensing reasons so upgrade it now
        ccache
        ninja

        # needed to build ffmpeg
        nasm
    )
    brew install "${brew_packages[@]}"

else # Linux & others

    if ! which sudo; then
        function sudo() { "$@"; } # no-op sudo for use in Docker images
    fi

    # Distribution packages
    if which apt-get; then
        apt_packages=(
            # Build tools
            file
            g++
            ninja-build
            nasm
            git
            wget
            bash
            scdoc
            ccache

            # Dependencies
            debhelper-compat
            gettext
            libasound2-dev
            libgtk-3-dev
            libsuil-dev

            # GitHub Actions
            gettext
        )
        sudo apt-get update -y
        sudo apt-get install -y --no-install-recommends "${apt_packages[@]}"
    else
        echo >&2 "$0: Error: You don't have a recognized package manager installed."
        exit 1
    fi
fi

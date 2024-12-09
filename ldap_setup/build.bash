#!/bin/bash

if [ "$#" -lt 1 ] || [ "$#" -gt 3 ]; then
    echo "Usage: $0 [astra|redos]"
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

configure_build() {
    local build_dir=$1
    local cmake_source_dir=$2
    local system_flag=$3

    if [ ! -d "$build_dir" ]; then
        echo "Creating directory $build_dir"
        mkdir -p "$build_dir"
    fi

    local cmake_flags="-DSYSTEM=$system_flag"

    echo "Configuring CMake for $build_dir"
    cmake -S "$cmake_source_dir" -B "$build_dir" $cmake_flags -DLIBPUGIXML=/home/LDAPSetup/libs/pugixml

    make -C "$build_dir"
}

system_flag=""

for arg in "$@"; do
    case $arg in
        astra|redos)
            system_flag=$arg
            ;;
        *)
            echo "Invalid argument: $arg"
            echo "Usage: $0 [app|test] [system]"
            exit 1
            ;;
    esac
done


configure_build "$SCRIPT_DIR/build_app" "$SCRIPT_DIR" "$system_flag"


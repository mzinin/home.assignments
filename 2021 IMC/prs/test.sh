#!/bin/bash

set -e

BUILD_TYPE=RELEASE

while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
        -m|--mode)
            BUILD_TYPE=$(echo "$2" | awk '{print toupper($0)}')
            shift
            shift
            ;;

        *)
            shift
            ;;
    esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
BUILD_SUB_DIR=$(echo "$BUILD_TYPE" | awk '{print tolower($0)}')
TESTS_DIR="${SCRIPT_DIR}/build/${BUILD_SUB_DIR}/test"

"${TESTS_DIR}/unit-tests" --order rand --durations yes

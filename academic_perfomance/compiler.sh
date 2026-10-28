#!/bin/bash
# Build script for the Academic Performance and Risk Analysis Engine.

set -e
cd "$(dirname "$0")"                  

OUT="academic"
SRCS="main.c input.c add_student.c display.c sorting.c avaraging.c top_student.c search.c remove.c perfomance_distribution.c summary.c"

case "$1" in
    clean)
        rm -f "$OUT"
        echo "Cleaned."
        exit 0
        ;;
esac

echo "Compiling..."
gcc -Wall -Wextra -std=c11 -g $SRCS -o "$OUT"
echo "Build successful: ./$OUT"

if [ "$1" = "run" ]; then
    ./"$OUT"
fi
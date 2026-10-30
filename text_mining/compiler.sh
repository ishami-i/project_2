#!/bin/bash
# Build script for the Dynamic Text Mining and Linguistic Pattern Analyzer.
set -e
cd "$(dirname "$0")"
gcc -Wall -Wextra -std=c11 -g main.c text_input.c word_extract.c operations.c analysis.c -o text_mining_app
echo "Build successful: ./text_mining_app"
[ "$1" = "run" ] && ./text_mining_app
exit 0


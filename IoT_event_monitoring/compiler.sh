#!/bin/bash
# Build script for the Dynamic IoT Event Monitoring and Callback Engine.
set -e
cd "$(dirname "$0")"
gcc -Wall -Wextra -std=c11 -g main.c input.c devices.c callbacks.c simulation.c report.c -o iot_monitor
echo "Build successful: ./iot_monitor   (optional: ./iot_monitor <seed>)"
[ "$1" = "run" ] && ./iot_monitor
exit 0
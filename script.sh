#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <tsp_file> <start_iteration> <end_iteration>"
    exit 1
fi

TSP_FILE="$1"
START_ITERATION="$2"
END_ITERATIONS="$3"

LOG_FILE="output.log"

# Remove the log file if it exists
if [ -f "$LOG_FILE" ]; then
    rm "$LOG_FILE"
fi

for ((i = START_ITERATION; i <= END_ITERATIONS; i++)); do

    echo "Iteration $i" >> "$LOG_FILE"

    for ((j = 0; j < 10; j++)); do

        ./main "$TSP_FILE" "$i" >> "$LOG_FILE" 2>&1

        wait $!

    done

    echo "Iteration $i done" >> "$LOG_FILE"

    sleep 0.1
done

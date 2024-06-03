#!/bin/bash

echo "Executing make..."
make


if [ $? -eq 0 ]; then
    echo "Make completed successfully."

    echo "Running script.sh with arguments wi19.tsp 1 256..."
    ./script.sh wi19.tsp 1 256

    if [ $? -eq 0 ]; then
        echo "script.sh executed successfully."
    else
        echo "script.sh failed to execute."
    fi
else
    echo "Make failed."
fi

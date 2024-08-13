#!/bin/bash

# Check if the source file exists
if [ ! -f "main.cpp" ]; then
    echo "Error: main.cpp not found!"
    exit 1
fi

# Get the directory name
DIRNAME=$(basename "$PWD")

# Log the start time
START_TIME=$(date +%s)

# Compile the main.cpp file
g++ main.cpp ./src/Auth.cpp ./src/database.cpp -o "$DIRNAME" -lpthread -lsqlite3 -lssl -lcrypto

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable created: $DIRNAME"
else
    echo "Compilation failed!"
    exit 1
fi

# Log the end time
END_TIME=$(date +%s)

# Calculate the duration
DURATION=$((END_TIME - START_TIME))

# Display the time taken
echo "Time taken to complete the task: $DURATION seconds"
#!/bin/bash

# Define the source and destination directories
SOURCE_DIR="/cyberman24/services/"
DEST_DIR="../src"

# Get the absolute path of the destination directory
ABS_DEST_DIR="$(cd "$(dirname "$0")" && cd "$DEST_DIR" && pwd)"

# Check if the source directory exists
if [ ! -d "$SOURCE_DIR" ]; then
  echo "Source directory $SOURCE_DIR does not exist."
  exit 1
fi

# Copy the content from the source to the destination
cp -r "$SOURCE_DIR"/* "$ABS_DEST_DIR"

# Check if the copy was successful
if [ $? -eq 0 ]; then
  echo "Data has been successfully copied from $SOURCE_DIR to $ABS_DEST_DIR."
else
  echo "Failed to copy data from $SOURCE_DIR to $ABS_DEST_DIR."
fi

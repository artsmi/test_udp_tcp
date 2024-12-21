#!/bin/bash

BUILD_DIRECTORY="./build"

if [ -d "$BUILD_DIRECTORY" ]; then
  echo "directory $BUILD_DIRECTORY does exist..."
else
  mkdir $BUILD_DIRECTORY
  echo "directory $BUILD_DIRECTORY was created..."
fi

cd $BUILD_DIRECTORY
cmake ..
make
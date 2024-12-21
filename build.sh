#!/bin/bash

function check_create {
  if [ -d "./$1" ]; then
    echo "directory ./$1 does exist..."
  else
    mkdir ./$1
    echo "directory ./$1 was created..."
  fi
}

BASE_DIRECTORY=$(pwd)
BUILD_DIRECTORY="build"
INSTALL_DIRECTORY="install"

check_create ${BUILD_DIRECTORY}
check_create ${INSTALL_DIRECTORY}

cd ${BUILD_DIRECTORY}

cmake -DGLOBAL_INSTALL_DIR="$BASE_DIRECTORY/${INSTALL_DIRECTORY}" ..
make
cmake --install ./

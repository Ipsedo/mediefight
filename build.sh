#!/bin/bash

## Managing default directories, args etc...

ORIGIN=$(pwd)

DEFAULT_CMAKE_BUILD_DIR="build"
BUILD_SUBDIR_NAME=".build_files"
EXEC_NAME="mediefight"

CMAKE_BUILD_DIR=${1}
OUTPUT_DIR=${2}

if [ -z "${CMAKE_BUILD_DIR}" ]; then
  CMAKE_BUILD_DIR=${DEFAULT_CMAKE_BUILD_DIR}
fi

## Making directories

mkdir -p ${CMAKE_BUILD_DIR}
mkdir -p ${CMAKE_BUILD_DIR}/release
mkdir -p ${CMAKE_BUILD_DIR}/debug

## BUILD

# Release
cd ${CMAKE_BUILD_DIR}/release
mkdir -p ${BUILD_SUBDIR_NAME}
cd ${BUILD_SUBDIR_NAME}
cmake -DCMAKE_BUILD_TYPE=Release ../../..
make
mv ${EXEC_NAME} ..
cd ..
objdump -dC ${EXEC_NAME} > ${EXEC_NAME}.asm

# Debug
cd ../debug
mkdir -p ${BUILD_SUBDIR_NAME}
cd ${BUILD_SUBDIR_NAME}
cmake -DCMAKE_BUILD_TYPE=Debug ../../..
make
mv ${EXEC_NAME} ..
cd ../
objdump -dC ${EXEC_NAME} > ${EXEC_NAME}.asm

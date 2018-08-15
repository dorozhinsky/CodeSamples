# !/bin/bash

export CXX=g++

echo "Starting building..."

# Check if requested to overwrite default compiler
if [ "$1" != '' ]; then
  CXX=$1
fi

echo "Compiler to be used: "$CXX
sleep 5
make clean > /dev/null
sleep 5
make > /dev/null

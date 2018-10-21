#!/bin/bash

port=$1
address=$2
numClients=$3

cmake .
make Client
for i in {1..${port}}
do
    ./Client cs

done


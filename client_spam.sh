#!/bin/bash

port=$1
address=$2
numClients=$3

cmake .
make Client
END=255
for ((i=1;i<=END;i++)); do
    ./Client Bot$i cs return.cs.umu.se 6969  > /dev/null 2>&1 &
done
sleep 3
pkill Client -9

#!/bin/bash

port=$1
address=$2
numClients=$3

cmake .
make Client
END=200
for ((i=1;i<=END;i++)); do
    ./Client Bot$i cs silverfox.cs.umu.se 6969 < first_message_before_exit.txt > /dev/null 2>&1 &
done
sleep 7
pkill Client -9

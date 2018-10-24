#!/bin/bash

port=$1
address=$2
numClients=$3

cmake .
make Client
END=260
for ((i=1;i<=END;i++)); do
    ./Client Bot$i cs long.cs.umu.se 1337 < first_message_before_exit.txt > /dev/null 2>&1 &
done
sleep 5
pkill Client -9

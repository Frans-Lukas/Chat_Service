#!/bin/bash

port=$1
address=$2
numClients=$3

cmake .
make Client
END=20
for ((i=1;i<=END;i++)); do
    ./Client BotBotterino$i cs long.cs.umu.se 6969  > /dev/null 2>&1 &
done
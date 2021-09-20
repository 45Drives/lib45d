#!/usr/bin/env bash

pids=()

cleanup() {
    kill ${pids[@]} 
    rm -f test.socket
    exit 1
}

trap 'cleanup' INT

./server.test > server.out 2>&1 &
pids+=$!

ps -p ${pids[0]} > /dev/null 2>&1 || exit 1

./client.test > client.out 2>&1
res=$?
if [[ "$res" != "0" ]]; then
    exit $res
fi

diff client.out client.gold
exit $?

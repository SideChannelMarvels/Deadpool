#!/bin/bash

while :; do
    p1=$(pgrep crackme.exe)
    sleep 5
    p2=$(pgrep crackme.exe)
    if [ "$p1" != "" ] && [ "$p1" == "$p2" ]; then
        echo "Killing $p1"
        xdotool search --name crackme windowkill
        sleep 2
    fi
done

#!/bin/bash

# Required only when using TracerPIN.
# For a mysterious reason, Go traced with PIN doesn't exit properly but needs repeted SIGTERM signals.

# Start sending SIGTERM to processes running for more than N seconds:
MAXTIME=1

echo "CTRL-C to interrupt"
while :; do
    for p in $(pgrep -x encryptECB); do
        if [ $(ps -o etimes $p|grep -v ELAPSED) -gt $MAXTIME ] 2>/dev/null; then
            kill $p
            echo "Killing $p"
        fi
    done
done

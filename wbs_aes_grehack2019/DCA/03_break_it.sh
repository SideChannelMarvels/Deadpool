#!/bin/bash

# focusing on first round: first 10% of traces
sed -i "s/nsamples=.*/nsamples=970/" bits_500_9704.config
# using the 500 available traces
sed -i "s/ [0-9]\+ / 500 /" bits_500_9704.config
for ((i=0; i<16; i++)); do
  sed -i "s/bytenum=.*/bytenum=$i/" bits_500_9704.config
  daredevil -c bits_500_9704.config
done

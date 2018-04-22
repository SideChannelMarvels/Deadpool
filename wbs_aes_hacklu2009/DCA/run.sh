#!/bin/bash

# Make sure to use the 32-bit version of Wine
valgrind --trace-children=yes --tool=tracergrind --filter=0x401000-0x401243 --vex-iropt-register-updates=allregs-at-mem-access --output=crackme.trace wine ../target/crackme.exe &
sleep 1
xdotool - << EOF
search --sync --limit 1 --name Crackme
sleep 0.1
key Tab Tab
key $*
key Tab space
sleep 0.1
windowkill
EOF

texttrace crackme.trace >(grep '^.M' > trace.tmp)
rm crackme.trace

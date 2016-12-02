## Usage

## Acquiring one trace with TracerGrind

Intel PIN should work also under Windows but TracerPIN was never tested under Windows so we choose to instrument Wine under Linux
with TracerGrind. (Intel PIN fails on Wine)

When an executable is launched via Wine, the Wine server is running in the background and we certainly don't want to instrument the whole Wine server!  
So, an easy way is to start anotehr process in a separate window and keep it open during the experiments with the crackme.

```bash
wine notepad.exe
```

Now we can trace our executable with an arbitrary input once we've identified the address range:

```bash
objdump -p ../target/crackme.exe |grep Image
ImageBase               00400000
SizeOfImage             00018000

valgrind --trace-children=yes --tool=tracergrind --filter=0x400000-0x401800 --vex-iropt-register-updates=allregs-at-mem-access --output=crackme.trace wine32 ../target/crackme.exe &
sleep 1
xdotool - << EOF
search --sync --limit 1 --name Crackme
sleep 0.1
key Tab Tab
key 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
key Tab space
sleep 0.1
windowkill
EOF

sqlitetrace crackme.trace crackme.sqlite
```

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph crackme.sqlite &
```

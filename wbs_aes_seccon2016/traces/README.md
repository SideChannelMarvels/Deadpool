## Acquiring one trace with TracerGrind

```bash
echo -n 0000000000000000|/usr/local/bin/valgrind --quiet --trace-children=yes --tool=tracergrind --filter=0x0-0x10000000 --vex-iropt-register-updates=allregs-at-mem-access --output=trace.grind ../target/main64 >/dev/null
sqlitetrace trace.grind trace.sqlite
```

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph trace.sqlite &
```

Instructions of the white-box are 0x407f000 to 0x4082000 and stack at 0xffeffe000.
These addresses are specific to how Valgrind loaded the dynamic library.

## Usage

## Acquiring one trace with TracerPIN


```bash
echo -n 00000000000000000000000000000000|Tracer -t sqlite -o rhme3.sqlite -- ../target/whitebox --stdin
```

## Acquiring one trace with TracerGrind

By default TracerGrind traces everything, so let's find the address range of the main executable and apply an address filter.

```bash
objdump -p ../target/whitebox |grep -A1 LOAD|grep -B1 "r.x"
    LOAD off    0x0000000000000000 vaddr 0x0000000000400000 paddr 0x0000000000400000 align 2**21
         filesz 0x000000000006432c memsz 0x000000000006432c flags r-x

echo -n 00000000000000000000000000000000|valgrind --tool=tracergrind --filter=0x400000-0x410000 --output=rhme3.trace ../target/whitebox --stdin
sqlitetrace rhme3.trace rhme3.sqlite
```

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph rhme3.sqlite &
```

Note that the memory layout is different when tracing with PIN or with Valgrind.

Let's use PIN, we see the instructions in the rounds loop range roughly from 0x462886 to 0x463D6C.

## Usage

## Acquiring one trace with TracerPIN


```bash
Tracer -t sqlite -- ../target/wbDES 11 22 33 44 55 66 77 88
```

## Acquiring one trace with TracerGrind

By default TracerGrind traces everything, so let's find the address range of the main executable and apply an address filter.

```bash
objdump -p ../target/wbDES |grep -A1 LOAD|grep -B1 "r.x"
    LOAD off    0x00000000 vaddr 0x08048000 paddr 0x08048000 align 2**12
         filesz 0x00002088 memsz 0x00002088 flags r-x

valgrind --tool=tracergrind --filter=0x08048000-0x08100000 --output=wbDES.trace ../target/wbDES 11 22 33 44 55 66 77 88
sqlitetrace wbDES.trace wbDES.sqlite
```

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph wbDES.sqlite &
```

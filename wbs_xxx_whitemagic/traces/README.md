## Usage

## Acquiring one trace with TracerPIN

Intel PIN doesn't seem to recognize the main executable image, so let's find its address range and apply an address filter.

```bash
objdump -p white_magic |grep -A1 LOAD|grep -B1 "r.x"
    LOAD off    0x0000000000000000 vaddr 0x0000000000400000 paddr 0x0000000000400000 align 2**21
         filesz 0x000000000001296c memsz 0x000000000001296c flags r-x

echo -n 0102030405060708 | xxd -r -p | Tracer -f 0x400000-0x600000 -t sqlite -- ./white_magic | xxd -p
```

## Acquiring one trace with TracerGrind

```bash
echo -n 0102030405060708 | xxd -r -p | valgrind --tool=tracergrind --filter=0x400000-0x600000 --output=whitemagic.trace ./white_magic | xxd -p
sqlitetrace whitemagic.trace whitemagic.sqlite
```

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph whitemagic.sqlite &
```

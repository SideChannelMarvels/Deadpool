## Usage

## Acquiring one trace with TracerPIN

```bash
Tracer -t sqlite -- ../target/encryptECB -key ../target/key.txt -in <(echo 000102030405060708090a0b0c0d0e0f|xxd -r -p) -out >(xxd -p)
```

The program exits too early for an unknown reason. So we'll switch to TracerGrind.

## Acquiring one trace with TracerGrind

By default TracerGrind traces everything, so let's find the address range of the main executable and apply an address filter.

```bash
objdump -p ../target/encryptECB |grep -A1 LOAD|grep -B1 "r.x"
    LOAD off    0x0000000000000000 vaddr 0x0000000000400000 paddr 0x0000000000400000 align 2**12
         filesz 0x00000000000e3810 memsz 0x00000000000e3810 flags r-x

valgrind --tool=tracergrind --filter=0x400000-0x500000 --output=xiaolai.trace ../target/encryptECB -key ../target/key.txt -in <(echo 000102030405060708090a0b0c0d0e0f|xxd -r -p) -out >(xxd -p)
sqlitetrace xiaolai.trace xiaolai.sqlite
```

Sqlite trace is about 500Mb large.

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph xiaolai.sqlite &
```

We may guess the AES is located at the bottom, where we see 10 successive large data reads.
Instructions are localized in several columns so for DCA we'll have to try each column:

* 0x40e2fe-0x40f025
* 0x43b12f-0x43b2ba
* 0x4755ba-0x475c90
* 0x4bcc50-0x4bd367


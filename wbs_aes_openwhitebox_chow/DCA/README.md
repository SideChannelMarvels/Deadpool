Usage
-----

Using TracerGrind to acquire execution traces, here recording the lowest byte of each address from which data is being read:

```bash
./trace_it.py
00000 4C24C189FF4A6564E67E00BC8A9F4A74 -> FA7B9828D862C9487DB88BE7EEA75ED8
00001 C149482EF16DD6136689F3D915478E87 -> 8E737A8CA6DFFA8612A98F4A40E754C7
00002 BB049542D61DB4CCD34BAF78A6C3177B -> CC08B6F55AAA719CA1BF60AC63BAB99A
00003 DD14D694EFECED4C73F3275C576ED8E9 -> 0FEC2A1046AAAFD2F221FC667A3B3009
00004 CE55E26322FAD1D8EDF6AFBC3DE81A34 -> A69407D4A36827720697C86489F773DF
...
```

It's possible to use TracerPIN instead of TracerGrind but there is one caveat as explained in ../traces/: we've to send several SIGTERM for the target to exit.
So this directory contains also an ugly watchdog.sh script to kill any target running for more than one second.

Executing the differential analysis on the converted traces:

```bash
daredevil -c mem_data_rw1_200_131176.attack_sbox.config
daredevil -c mem_data_rw1_200_131176.attack_multinv.config
```

Usage
-----


Using TracerPIN to acquire execution traces, here recording the lowest byte of each address from which data is being read:

```bash
./trace_it.py
00000 7B75AD7B1ABA8EEF70B0DAA8BFD0545F -> E502FC91A3332AB77215762DE74F6DB3
00001 95578FE9BCD44B77FBDB2A5F2198D2D6 -> 060FBE3E501FFE8BE925C9167C1F2476
00002 182653079A8EBAFF7ECCAC53D0FE9640 -> D39143D325E056312E563B92C1336FB8
00003 94BBEC8B80D68A228F6E3A221FCA14F9 -> AFBD5DB355C4A7B0F166B0F975875DC5
00004 B5568F7E506D44C7E213FBEF421C6304 -> B2CFEAE2AF6B8AE5D6D5007170370312
...
```

When looking to the graph of a trace (see [traces](../traces)), we see the instructions in the rounds loop range roughly from 0x462886 to 0x463D6C, so ```trace_it.py``` is limiting the trace to that portion of code. It's not required but just to be faster.

Executing the differential analysis on the converted traces:

```bash
daredevil -c mem_addr1_rw1_32_7976.config
Most probable key max(abs):
1: 16: 61316c5f7434623133355f525f6f5235
```

Total attack takes 13s.

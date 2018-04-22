Usage
-----

First go to the ```target``` directory and compile the white-box.

Using TracerGrind to acquire execution traces on address range 0x108000-0x130000, here recording the lowest byte of each address from which data is being read:

```bash
./trace_it.py
00000 B59FB62E3E4A1DC2260F51C2B52E9C9A -> 2EA4F0D863CE0497CCDB463AA33951F5
00001 9B8FFB8174BF3295A9F6CC0E995ADA4A -> C3BDDEF6CFA8AC6F80DA2222D415EA6C
00002 5AAFA5030BC30835629A1015B0D23C12 -> 3CE85EAEB8D93E658AED19176AA21A3F
00003 B6BD4D36210C1A5500A9006EA92640DD -> F396AEC95428972197561025A35A329B
00004 D71522ADC0A8AC8394317BF2E9C78F57 -> 24824B6C519DD9CF55DD02C010EDBC0B
...
```

By default the script is using TracerGrind but if you want you can change it to use TracerPIN

Executing the differential analysis on the converted traces...

By the time of writing, Daredevil doesn't break all key bytes at once but only one key byte.
Default generated configuration attacks the first key byte:

```bash
daredevil -c mem_addr1_rw1_2000_32400.attack_sbox.config

Best 10 candidates for key byte #0 according to sum(abs(bit_correlations)):
 0: 0xde  sum: 1.52317 
 1: 0xb6  sum: 1.45995 
 2: 0xe4  sum: 1.43009 
 3: 0xc0  sum: 1.42041 
 4: 0xe1  sum: 1.41698 
 5: 0x2b  sum: 1.40646 
 6: 0xdf  sum: 1.40392 
 7: 0x28  sum: 1.39438 
 8: 0xc9  sum: 1.39343 
 9: 0x6d  sum: 1.38268 

Best 10 candidates for key byte #0 according to highest abs(bit_correlations):
 0: 0xde peak: 0.492984
 1: 0x4c peak: 0.268989
 2: 0xdf peak: 0.253557
 3: 0xb6 peak: 0.247341
 4: 0xfc peak: 0.240305
 5: 0xab peak: 0.239221
 6: 0x3f peak: 0.234549
 7: 0xe4 peak: 0.231214
 8: 0x3d peak: 0.229994
 9: 0xc9 peak: 0.229779
```

Key byte 0 = ```0xDE``` stands clearly out with a good correlation margin.

Then edit ```mem_addr1_rw1_2000_32400.attack_sbox.config``` and change ```bytenum``` with values from 0 to 15.

```
Best 10 candidates for key byte #1 according to highest abs(bit_correlations):
 0: 0xc1 peak: 0.54093   <==
 1: 0x59 peak: 0.267114
 2: 0xea peak: 0.261505
Best 10 candidates for key byte #2 according to highest abs(bit_correlations):
 0: 0xa5 peak: 0.278119  <== ? yes
 1: 0x57 peak: 0.262358
 2: 0x05 peak: 0.253499
Best 10 candidates for key byte #3 according to highest abs(bit_correlations):
 0: 0x51 peak: 0.501364  <==
 1: 0x5f peak: 0.249909
 2: 0xa6 peak: 0.249532
Best 10 candidates for key byte #4 according to highest abs(bit_correlations):
 0: 0xf1 peak: 0.548952  <==
 1: 0xc1 peak: 0.280923
 2: 0x91 peak: 0.275681
Best 10 candidates for key byte #5 according to highest abs(bit_correlations):
 0: 0xb1 peak: 0.294209  <== ? no
 1: 0xed peak: 0.288612  <==
 2: 0x4c peak: 0.286076
Best 10 candidates for key byte #6 according to highest abs(bit_correlations):
 0: 0xde peak: 0.27959   <== ? yes
 1: 0x07 peak: 0.267942
 2: 0x40 peak: 0.265024
Best 10 candidates for key byte #7 according to highest abs(bit_correlations):
 0: 0xc0 peak: 0.50393   <==
 1: 0x2e peak: 0.275168
 2: 0x89 peak: 0.267862
Best 10 candidates for key byte #8 according to highest abs(bit_correlations):
 0: 0xde peak: 0.495831  <==
 1: 0x12 peak: 0.2679  
 2: 0xd8 peak: 0.253003
Best 10 candidates for key byte #9 according to highest abs(bit_correlations):
 0: 0x4b peak: 0.478978  <==
 1: 0x98 peak: 0.25691 
 2: 0x50 peak: 0.24501 
Best 10 candidates for key byte #10 according to highest abs(bit_correlations):
 0: 0x1d peak: 0.771031  <==
 1: 0xe5 peak: 0.287954
 2: 0xd1 peak: 0.26425 
Best 10 candidates for key byte #11 according to highest abs(bit_correlations):
 0: 0xae peak: 0.492921  <==
 1: 0x15 peak: 0.254949
 2: 0x30 peak: 0.252902
Best 10 candidates for key byte #12 according to highest abs(bit_correlations):
 0: 0x5c peak: 0.533928  <==
 1: 0xca peak: 0.28905 
 2: 0x7e peak: 0.262977
Best 10 candidates for key byte #13 according to highest abs(bit_correlations):
 0: 0x07 peak: 0.308815  <== ? no
 1: 0x0d peak: 0.277734  <==
 2: 0xd1 peak: 0.250025
Best 10 candidates for key byte #14 according to highest abs(bit_correlations):
 0: 0xe5 peak: 0.742996  <==
 1: 0x3a peak: 0.260856
 2: 0xec peak: 0.244827
Best 10 candidates for key byte #15 according to highest abs(bit_correlations):
 0: 0x11 peak: 0.526463  <==
 1: 0x96 peak: 0.249488
 2: 0x30 peak: 0.246954
```

12 of the 16 key bytes got clearly broken.
Some (#2, #5, #6, #13) don't have a good correlation (actually some were still ok: #2 & #6 and some were ranked second: #5 & #13).

Now trying the multinv target:

```bash
daredevil -c mem_addr1_rw1_2000_32400.attack_multinv.config
```
Edit ```bytenum``` value and run it again for each key byte, till you get:

```
Best 10 candidates for key byte #0 according to highest abs(bit_correlations):
 0: 0xde peak: 0.767003  <==
 1: 0x40 peak: 0.262667
 2: 0xe7 peak: 0.258962
Best 10 candidates for key byte #1 according to highest abs(bit_correlations):
 0: 0xc1 peak: 0.510545  <==
 1: 0x2a peak: 0.265969
 2: 0x91 peak: 0.254192
Best 10 candidates for key byte #2 according to highest abs(bit_correlations):
 0: 0xa5 peak: 0.507505  <==
 1: 0xdd peak: 0.277941
 2: 0x10 peak: 0.261867
Best 10 candidates for key byte #3 according to highest abs(bit_correlations):
 0: 0x51 peak: 0.491318  <==
 1: 0x13 peak: 0.27094 
 2: 0x4b peak: 0.262211
Best 10 candidates for key byte #4 according to highest abs(bit_correlations):
 0: 0xf1 peak: 0.494083  <==
 1: 0x1e peak: 0.276092
 2: 0x24 peak: 0.276075
Best 10 candidates for key byte #5 according to highest abs(bit_correlations):
 0: 0xed peak: 0.530584  <==
 1: 0xc3 peak: 0.262888
 2: 0xe2 peak: 0.256592
Best 10 candidates for key byte #6 according to highest abs(bit_correlations):
 0: 0xde peak: 0.511036  <==
 1: 0xbc peak: 0.262117
 2: 0xc3 peak: 0.247025
Best 10 candidates for key byte #7 according to highest abs(bit_correlations):
 0: 0xc0 peak: 0.253988  <== ? yes
 1: 0x1f peak: 0.25278 
 2: 0xb6 peak: 0.242939
Best 10 candidates for key byte #8 according to highest abs(bit_correlations):
 0: 0xde peak: 0.793094  <==
 1: 0x3e peak: 0.259806
 2: 0x21 peak: 0.247518
Best 10 candidates for key byte #9 according to highest abs(bit_correlations):
 0: 0x37 peak: 0.259985  <== ? no
 1: 0x4e peak: 0.247819
 2: 0x90 peak: 0.247819
Best 10 candidates for key byte #10 according to highest abs(bit_correlations):
 0: 0x1d peak: 0.528998  <==
 1: 0x5b peak: 0.275102
 2: 0x55 peak: 0.271617
Best 10 candidates for key byte #11 according to highest abs(bit_correlations):
 0: 0xae peak: 0.512661  <==
 1: 0xfe peak: 0.282045
 2: 0x47 peak: 0.270365
Best 10 candidates for key byte #12 according to highest abs(bit_correlations):
 0: 0x5c peak: 0.533987  <==
 1: 0x61 peak: 0.259003
 2: 0xbb peak: 0.250361
Best 10 candidates for key byte #13 according to highest abs(bit_correlations):
 0: 0x0d peak: 0.507931  <==
 1: 0xb8 peak: 0.258857
 2: 0xcc peak: 0.25593 
Best 10 candidates for key byte #14 according to highest abs(bit_correlations):
 0: 0x26 peak: 0.286391  <== ? no
 1: 0xf6 peak: 0.249247
 2: 0x07 peak: 0.249021
Best 10 candidates for key byte #15 according to highest abs(bit_correlations):
 0: 0x11 peak: 0.73701   <==
 1: 0x27 peak: 0.268855
 2: 0xda peak: 0.247685
```

12 of the 16 key bytes got clearly broken.
Some (#7, #9, #14) don't have a good correlation (actually some were still ok: #7).

So by combining both attacks, we can recover the full key without any doubt:
```
dec1a551f1eddec0de4b1dae5c0de511
```

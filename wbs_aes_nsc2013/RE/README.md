Context
-------

The principle of this challenge is that you provide a name and a 16-byte hex serial that should match.
Internally the serial goes through a white-box and the output is compared to MD5(name).

To keep the story short, this white-box is an AES with external encodings which are **not** compensated locally,
so the serial is considered as already encoded and the output is not decoded before being compared to the hash.
So it is not performing AES(serial)==MD5(name) but ENC(AES(DEC(serial)))==MD5(name)  
Given that, classical DCA and DFA can't work as we never know the corresponding AES input or output.

So we've to use the long path: reverse-engineering then applying algebraic attacks.
This directory contains the reverse-engineering part.
The algebraic attack proposed in ALG only requires some of the tables to be extracted but we'll go up to getting a fully working white-box so you may try other attacks on that one.

Once the challenge was over, Eloi Vanderbéken [published](https://twitter.com/elvanderb/status/341542457172951040) his white-box generator, which is much cleaner to work with than the original challenge.
But it contains only the generator, neither the actual key used in the challenge nor the generated tables.
On the other side, Axel Souchet went up to isolating the white-box rounds to brute-force each round input till finding a suitable white-box input that produces the desired output.

Table extraction
----------------

So the first step to ease attacks on the white-box part itself is to conciliate the two versions mentioned above.

To do so, we reuse [Axel Souchet's write-up](http://0vercl0k.tuxfamily.org/bl0g/?p=253) to have some useable source code from his solver.
You may follow his write-up till you obtain [wbaes128_solve.cpp](https://github.com/0vercl0k/stuffz/blob/master/NoSuchCon2013/wbaes128_solve/wbaes128_solve.cpp)
and [wbaes128.h](https://github.com/0vercl0k/stuffz/blob/master/NoSuchCon2013/wbaes128/wbaes128.h) which are reproduced in this repository for simplicity.

We perform some operations on those files to get tables suitable for [Eloi's generator](http://pastebin.com/MvXpGZts).  
Those steps are detailed in http://wiki.yobi.be/wiki/NSC_Writeups#Epilogue  
```nosuchcon_2013_whitebox_debug.c``` and ```result/nosuchcon_2013_whitebox.c``` are derived from that generator.
At this point we'll get a clean version of the white-box, as if it was produced by Eloi's generator.
A script is automating the required steps:

```bash
./extract.sh
```

Usage
-----

Now we can use the white-box dropped in the result directory:

```bash
cd result
./nosuchcon_2013_whitebox c9dcf0f471fd85854eb255e8a80daaad
Enc in:   C9 DC F0 F4 71 FD 85 85 4E B2 55 E8 A8 0D AA AD 
Enc out:  EE 87 0C D1 40 8B 7A 09 F8 3D BA 4B 1E 8C 81 B5 
```

Here we took as input the serial used in Axel's write-up and we find the output equal to the MD5(name):

```bash
echo -n "AES Whitebox Unboxing: No Such Problem - 0vercl0k"|md5sum
ee870cd1408b7a09f83dba4b1e8c81b5  -
```

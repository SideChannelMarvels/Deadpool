(copy from https://gist.github.com/nreboud/863b9e49b5584cc6d6bea8f13aab3e05)

# GreHack2019 CTF challenge

One of the challenges of the [GreHack2019](https://grehack.fr/) CTF was a white-box.
The White-box has been pushed to the [SideChannelMarvels Project](https://github.com/SideChannelMarvels/Deadpool/tree/master/wbs_aes_grehack2019).

### 1. White-box? What is that?
White-box cryptography is presented here : http://www.whiteboxcrypto.com/. If you are not familiar with the concept, Brecht Wyseur introduced it well in ["white-box cryptography: hiding keys in software"](http://www.whiteboxcrypto.com/files/2012_misc.pdf), MISC magazine, April 2012

### 2. GreHack2019 White-box
The GreHack2019 CTF white-box is very classic [AES128](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf) white-box implementation. The fact that it implements an AES can be found by having a look to the code and by identifying the 11 rounds or by simply looking to the input sample ["Who Is Rijndael"](https://en.wikipedia.org/wiki/AES_key_schedule).

The algorithm used in the white-box implementation is mostly straightforward. The state is divided into 32 nibbles of 4 bits that are encodded separately. The encoding functions are randoms 16-byte permutations. They are refreshed after each use.
Each 256-byte table tabulates a basic operation. For instance, the addRoundKey operation is implemented as follows:
For i in [0, 255]:
 - Decode the two nibbles of _i_ to obtain the decoded _i_ value.
 - Perform a XOR operation with the subkey byte: _i'_ = _i_ ^ _k_
 - Encode each nibble of _i'_ with new encoding functions.

### 3. OK but how do I break it?
In the allotted time, the easiest way to break it was probably a DFA using the wellwbs_aes_ches2017_777-known "Piret and Quisquater 2003" or its revisited by Giraud and Thillard [version](https://eprint.iacr.org/2010/440.pdf).

The state has 5 additional bytes mostly used for mixColumn operations, but it is always handled in order. The positions of the different bytes of the state are not mixed. Therefore, on the first rounds we always have:
- [0, 15] -> state of the AES,
- [16; 20] -> additional variables,
- [21; 42] -> not used in the first rounds.
 
There is a small anti-DFA mechanism that protects the 3 last rounds of the AES: the state is doubled before entering the mixColumn of round 8.
```
s[41] = t_696[s[0]];
s[40] = t_697[s[1]];
s[39] = t_698[s[2]];
[...]
s[23] = t_714[s[18]];
s[22] = t_715[s[19]];
s[21] = t_716[s[20]];
```

From this moment, all the operations are doubled and we used both bytes of the state and bytes of the redundancy state (positions 21 to 42).
After the last round, the following operations are performed: 
 - each byte _state[i]_ of the state is xored with _state[(i+1)%15]_ and its mirror _state[41 - ((i+1)%15)]_
 - last addRounKey
 - compare each byte of the state and its mirror. If they are equal, return it else add it modulo 15.

To circumvent this countermeasure it is thus necessary to inject a fault on a byte of the state and on its mirror. Since the encoding functions are different, we have 1 chance on 256 of obtaining a usable faulted output.

### DCA
As there is no masking, DCA attacks have a high probability of success, but they require adequate tools and advanced knowledge. It is using a DCA attack that the only team that managed the challenge proceeded. For more information, visit the [Side Channel Marvel Project page](https://github.com/SideChannelMarvels/Deadpool/tree/master/wbs_aes_grehack2019).

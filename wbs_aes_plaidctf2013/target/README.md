Usage
-----

Original usage:

```bash
tar xf drmless.tgz-e9f85853ac856d7ed7a5a8c6e807955f07bbfa7a
cd drmless
./drmless cool_story.enc
```

```drmless``` is a modified ```less``` taking a ```.drmlicense``` file containing what is actually a XOR key.  
The *.enc files are encrypted with an unknown AES-128-ECB and XORed with a license file.  
Internally ```drmless``` tries to decrypt the first four blocks with its internal white-box and xor wit the license file, then checks if it looks like ASCII before outputting the whole file.

To prepare for experiments, extract ```drmless```:

```bash
tar xf drmless.tgz-e9f85853ac856d7ed7a5a8c6e807955f07bbfa7a drmless/drmless --strip-components 1
```

Solution
--------

Here is the equivalent of drmless, written in Python and with the correct AES key:

```python
#!/usr/bin/env python3
from Crypto.Cipher import AES
import sys
import itertools
xor = lambda data, key: bytes(i^j for i, j in zip(data, itertools.cycle(key)))
data = open(sys.argv[1], 'rb').read()
aes = AES.new(b'\xb1\xba\x27\x37\xc8\x32\x33\xfe\x7f\x7a\x7d\xf0\xfb\xb0\x1d\x4a', AES.MODE_ECB)
xorkey = open('.drmlicense', 'rb').read()
open(sys.argv[1]+'.txt', 'wb').write(xor(aes.decrypt(data), xorkey))
```

```bash
./drmless.py cool_story.enc
```

To decrypt cooler_story.enc, you need to provide the right license key.
The XOR key is revealed in several writeups:

```python
xorkey = b"\xfe\xed\xa1\x07\x0f\xf0\rp\xde\xad\xbe\xef\xfa\xceUU"
```

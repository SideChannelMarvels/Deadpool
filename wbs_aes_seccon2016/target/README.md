Compilation
-----------

The target is a whitebox in a native library bundled in an Android application.
The native library is available for several platforms: ARM, ARM64, x86, x86-64.

Thanks to [LIEF](https://lief.quarkslab.com/) (thanks Romain!), we'll make the x86-64 version compatible with a regular Linux!

```bash
unzip OAES.zip
unzip -j OAES_h2spice.apk lib/x86_64/libnative-lib.so
rm OAES_h2spice.apk
./lief_transform.py
g++ main.cpp -ldl -o main64
```

Usage
-----

```bash
echo -n 534543434f4e7b6f77536b7750654831|xxd -r -p|./main|xxd -p
83552565a7e2b867428294e45988da66
```

Solution
--------

```bash
echo 83552565a7e2b867428294e45988da66|xxd -r -p|openssl enc -d -aes-128-ecb -nopad -K 6C2893F21B6185E8567238CB78184945|xxd -p
534543434f4e7b6f77536b7750654831

echo g1UlZafiuGdCgpTkWYjaZg3kE6qCd7kF3kV+nMKcGHc=|base64 -d|openssl enc -d -aes-128-ecb -nopad -K 6C2893F21B6185E8567238CB78184945
SECCON{owSkwPeH1CHQdPV9KWrSmz9n}
```

BGE attack
----------

We'll perform a BGE attack on intermediate rounds 1, 2, 3, 4 and 5 with the tool [BlueGalaxyEnergy](https://github.com/SideChannelMarvels/BlueGalaxyEnergy).

Installation:
```bash
pip install bluegalaxyenergy
```

To run the attack, we compile the whitebox as a shared library

```bash
gcc -o libgrehack2019.so -shared -fPIC ../target/enc.c -O0
```

And we will trace it with QBDI, which must be installed too

```bash
pip install PyQBDI
```

Attack
------

```bash
./runme.py libgrehack2019.so
key: 474831397b4145532069732046554e7d
```
```
echo 474831397b4145532069732046554e7d|xxd -r -p
GH19{AES is FUN}
```

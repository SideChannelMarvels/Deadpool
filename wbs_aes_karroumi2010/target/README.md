Compilation
-----------

```bash
git clone https://github.com/ph4r05/Whitebox-crypto-AES.git
apt-get install libntl-dev libboost-iostreams1.58-dev libboost-program-options1.58-dev libboost-random1.58-dev libboost-serialization1.58-dev
cd Whitebox-crypto-AES
cmake .
make
```

You'll find here the result of the compilation of a slightly patched version to re-introduce the optimization flag in CMakeLists.txt, from commit b59be34f:

```diff
-set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
+set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++0x")
```

Usage
-----

By defaut it generates tables then encrypts input.  
We can also separate both steps, which is more representative of a whitebox and its generator.

Generating tables (optionally with a specific key ```--use-key ...```):

```bash
./main --create-table tables_karroumi_noextenc.tbl
```

Using the white-box:

```bash
./main --load-tables tables_karroumi_noextenc.tbl --input-files <(echo 6bc1bee22e409f96e93d7e117393172a|xxd -r -p) --out-file >(xxd -p)
3ad77bb40d7a3660a89ecaf32466ef97
```

Solution
--------

```bash
echo 6bc1bee22e409f96e93d7e117393172a|xxd -r -p|openssl enc -e -aes-128-ecb -nopad -K 2b7e151628aed2a6abf7158809cf4f3c |xxd -p
3ad77bb40d7a3660a89ecaf32466ef97
```

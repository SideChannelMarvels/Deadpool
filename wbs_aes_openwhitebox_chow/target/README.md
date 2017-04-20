Compilation
-----------

This repository contains already both executables so you can use them without installing Go

They were generated on a Debian testing/unstable:

```bash
apt-get install golang
export GOPATH=$(pwd)
mkdir -p src/github.com/OpenWhiteBox
cd src/github.com/OpenWhiteBox
git clone https://github.com/OpenWhiteBox/AES.git
cd AES
git checkout 817c3e6
cd examples/chow-file-encrypt
```

Drop encryptECB.go there, a version of encrypt.go which processes just one block.

Compile the generator and the whitebox example:

```bash
go build -buildmode=exe generate-key.go
go build -buildmode=exe encryptECB.go
```

Usage
-----

```bash
./generate-key
Key: 693bb79cd7742262c969595c4f8d895f
```

Whitebox key data is created in `key.txt`.

Then to use it:

```bash
./encryptECB -in <(echo 000102030405060708090a0b0c0d0e0f|xxd -r -p) -out >(xxd -p)
99d527e5d281e9166825581337dac9c8
```

Solution
--------

```bash
echo 99d527e5d281e9166825581337dac9c8|xxd -r -p|openssl enc -d -aes-128-ecb -K 693bb79cd7742262c969595c4f8d895f -nopad|xxd -p
000102030405060708090a0b0c0d0e0f
```

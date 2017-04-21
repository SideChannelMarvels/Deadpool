# Compilation

This repository contains already the executables so you can use them without installing Go

They were generated on a Debian testing/unstable:

```bash
apt-get install golang
export GOPATH=$(pwd)
mkdir -p src/github.com/OpenWhiteBox
cd src/github.com/OpenWhiteBox
git clone https://github.com/OpenWhiteBox/AES.git
# using commit b7fcb3c
git clone https://github.com/OpenWhiteBox/primitives.git
# using commit 2f25eea
cd AES/example
mv encrypt.go encrypt_remoteextenc.go
```

Drop encrypt_localextenc.go there, a version of encrypt.go which applies locally the external encodings. (Thank you Brendan!)


Compile the generator and the whitebox examples:

```bash
go build -buildmode=exe generate_key.go
go build -buildmode=exe encrypt_remoteextenc.go
go build -buildmode=exe encrypt_localextenc.go
```

# Usage

## Generator

Here are two versions of the OpenWhiteBox AES challenge. The generator is the same for both versions:

```bash
./generate_key -key 2b7e151628aed2a6abf7158809cf4f3c
```

This generates `constr.key`, what the authors call the _private_ part, containing the external encodings, and `constr.txt`, what they call the _public_ part, containing the whitebox tables.

## Remotely-applied external encodings version

In this version, only `constr.txt` is provided and therefore the input provided to `encrypt_remoteextenc` is supposed to be already encoded while the output still needs to be decoded.

To use it:

```bash
./encrypt_remoteextenc -block 6bc1bee22e409f96e93d7e117393172a
1656eeab454bc6ccfa36329da09ddddd
```

###Solution

We can't check the input-output relationship as we don't know the external encodings in this configuration.

## Locally-applied external encodings version

In this version, `constr.txt` and `constr.key` are provided and `encrypt_localextenc` applies locally the input and output external encodings, therefore `encrypt_localextenc` acts as a standard AES. 

To use it:

```bash
./encrypt_localextenc -block 6bc1bee22e409f96e93d7e117393172a
3ad77bb40d7a3660a89ecaf32466ef97
```

###Solution

```bash
echo 3ad77bb40d7a3660a89ecaf32466ef97|xxd -r -p|openssl enc -d -aes-128-ecb -K 2b7e151628aed2a6abf7158809cf4f3c -nopad|xxd -p
6bc1bee22e409f96e93d7e117393172a
```

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
sed -i 's/chow/xiao/g' *.go
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
Key: 95b600e4b8e05d6a9d2b59993835dcd8
```

Whitebox key data is created in `key.txt`.

Then to use it:

```bash
./encryptECB -in <(echo 000102030405060708090a0b0c0d0e0f|xxd -r -p) -out >(xxd -p)
4317dfcb38e8f9a1c00628dfb652cdad
```

Solution
--------

```bash
echo 4317dfcb38e8f9a1c00628dfb652cdad|xxd -r -p|openssl enc -d -aes-128-ecb -K 95b600e4b8e05d6a9d2b59993835dcd8 -nopad|xxd -p
000102030405060708090a0b0c0d0e0f
```

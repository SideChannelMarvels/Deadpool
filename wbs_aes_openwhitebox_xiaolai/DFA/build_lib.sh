#!/usr/bin/env bash

set -ex

SCRIPTPATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

export GOPATH="$(pwd)"
go env -w GO111MODULE=off

mkdir -p "${SCRIPTPATH}/src/github.com/OpenWhiteBox/"

if [[ ! -e "${SCRIPTPATH}/src/github.com/OpenWhiteBox/AES" ]]; then
  cd "${SCRIPTPATH}/src/github.com/OpenWhiteBox/"
  git clone https://github.com/OpenWhiteBox/AES.git
  cd AES
  git checkout 817c3e6
  cd examples/chow-file-encrypt
  sed -i 's/chow/xiao/g' *.go
  go build -buildmode=exe generate-key.go
  cp generate-key "${SCRIPTPATH}"
  cd "${SCRIPTPATH}"
fi

if [[ ! -e "${SCRIPTPATH}/encryptECB" ]]; then
  go build -buildmode=exe encryptECB.go
fi

if [[ ! -e "${SCRIPTPATH}/key.txt" ]]; then
  ./generate-key > raw_key.txt
  chmod 444 "${SCRIPTPATH}/key.txt"
fi

go build -buildmode=c-archive encryptECBlib.go

gcc -o libwbxiaolai.so -shared -fPIC encryptECBinterface.c -L. -l:encryptECBlib.a

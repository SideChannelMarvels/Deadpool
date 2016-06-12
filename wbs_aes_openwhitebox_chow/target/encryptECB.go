package main

import (
	"io/ioutil"
	"os"
	"github.com/OpenWhiteBox/AES/constructions/chow"
)

func main() {
	keyData, _ := ioutil.ReadFile("key.txt")
	block, _ := chow.Parse(keyData)
	data, _ := ioutil.ReadFile("secrets.txt")
	block.Encrypt(data,data)
	ioutil.WriteFile("secrets.enc", data, os.ModePerm)
}

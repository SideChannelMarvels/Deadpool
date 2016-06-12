package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"os"

	"github.com/OpenWhiteBox/AES/constructions/chow"
)

var (
	key = flag.String("key", "key.txt", "White-Box AES Key to encrypt with.")
	in  = flag.String("in", "secrets.txt", "Input file to encrypt.")
	out = flag.String("out", "secrets.enc", "Output file to write the encrypted file to.")
)

func main() {
	flag.Parse()

	// Load in key data and initialize the block cipher.
	keyData, err := ioutil.ReadFile(*key)
	if err != nil {
		panic(err)
	}

	block, _ := chow.Parse(keyData)

	// Load in file to encrypt.
	data, err := ioutil.ReadFile(*in)
	if err != nil {
		panic(err)
	}

	// Encrypt file.
	block.Encrypt(data,data)

	// Write encrypted file to disk.
	ioutil.WriteFile(*out, data, os.ModePerm)

	fmt.Println("Done!")
}

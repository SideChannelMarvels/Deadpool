// Command encrypt_correct reads a block from the command line, encrypts it with
// the white-box, and outputs the encrypted block.
//
// It uses the private part of the white-box to scrub the random input/output
// transformations, in order to agree with standard AES.
//
// Example:
//   $ go run encrypt_correct.go -block 000000000000000000000000deadbeef
//   e99447e5eb535c5cd0baa5aa5151d7c9
package main

import (
	"encoding/hex"
	"flag"
	"fmt"
	"io/ioutil"
	"log"

	"github.com/OpenWhiteBox/AES/constructions/full"
	"github.com/OpenWhiteBox/primitives/encoding"
	"github.com/OpenWhiteBox/primitives/matrix"
)

const keySize = 16 * (1 + 128 + 1 + 128 + 1)

var hexBlock = flag.String("block", "", "A hex-encoded 128-bit block to encrypt.")

func main() {
	flag.Parse()
	block, err := hex.DecodeString(*hexBlock)
	if err != nil {
		log.Println(err)
		flag.PrintDefaults()
		return
	} else if len(block) != 16 {
		log.Println("Block must be 128 bits.")
		flag.PrintDefaults()
		return
	}

	// -- Key parsing

	// Read public key from disk.
	dataPub, err := ioutil.ReadFile("./constr.txt")
	if err != nil {
		log.Fatal(err)
	}
	constr, err := full.Parse(dataPub)
	if err != nil {
		log.Fatal(err)
	}

	// Read private key from disk.
	dataPriv, err := ioutil.ReadFile("./constr.key")
	if err != nil {
		log.Fatal(err)
	} else if len(dataPriv) != keySize {
		log.Fatalf("key wrong size: %v (should be %v)", len(dataPriv), keySize)
	}
	dataPriv = dataPriv[16:]

	inputLinear, outputLinear := matrix.Matrix{}, matrix.Matrix{}
	inputConst, outputConst := [16]byte{}, [16]byte{}

	for i := 0; i < 128; i++ {
		inputLinear, dataPriv = append(inputLinear, dataPriv[:16]), dataPriv[16:]
	}
	copy(inputConst[:], dataPriv)
	dataPriv = dataPriv[16:]
	for i := 0; i < 128; i++ {
		outputLinear, dataPriv = append(outputLinear, dataPriv[:16]), dataPriv[16:]
	}
	copy(outputConst[:], dataPriv)

	inputMask := encoding.NewBlockAffine(inputLinear, inputConst)
	outputMask := encoding.NewBlockAffine(outputLinear, outputConst)

	// -- Encryption

	temp := [16]byte{}
	copy(temp[:], block)

	temp = inputMask.Decode(temp)
	constr.Encrypt(temp[:], temp[:])
	temp = outputMask.Decode(temp)

	fmt.Printf("%x\n", temp)
}
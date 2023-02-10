package main

import (
  "C"

	"github.com/OpenWhiteBox/AES/constructions/xiao"
  "github.com/OpenWhiteBox/AES/primitives/matrix"
)

var keyFile[]byte
var keyTable = xiao.Construction{}

//export goParseKeyData
func goParseKeyData(in *[]byte){
  // copy the slice to keep it available
  keyFile = make([]byte, len(*in))
  copy(keyFile, *in)

	key, err := xiao.Parse(keyFile)
	if err != nil {
		panic(err)
	}
  keyTable = key
  return 
}

//export goRunWhiteBox
func goRunWhiteBox(inOut *[]byte) {
	keyTable.Encrypt(*inOut, *inOut)
}

//export goShiftRows
func goShiftRows(inOut *[]byte, round int) {
  dst := make([]byte, 16)
  copy(dst, *inOut)

  if round == 10 {
    copy(*inOut, keyTable.FinalMask.Mul(matrix.Row(dst)))
  } else if round > 10 {
		panic("Invalid round")
  } else {
    // ShiftRows and re-encoding step.
    copy(*inOut, keyTable.ShiftRows[round].Mul(matrix.Row(dst)))
  }
  return
}

//export goMixColumns
func goMixColumns(inOut *[]byte, round int) {
  dst := make([]byte, 16)
  copy(dst, *inOut)

  if round >= 10 {
		panic("Invalid round")
  } else {
    // Apply T-Boxes and MixColumns
    for pos := 0; pos < 16; pos += 4 {
      stretched := keyTable.ExpandWord(keyTable.TBoxMixCol[round][pos/2:(pos+4)/2], dst[pos:pos+4])
      keyTable.SquashWords(stretched, dst[pos:pos+4])
    }
    // ShiftRows and re-encoding step.
    copy(*inOut, dst)
  }
  return
}


func main() {}

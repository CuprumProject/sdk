// Copyright (c) 2021, CuprumProject author.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

package main

import(
  "bytes"
  "fmt"
  "os/exec"
  "strconv"
  "os"
)

type Errors int
const (
  /* USB Errors */
  EXCEPT_FILE_NOT_FOUND Errors = 1
)

const (
  kReadOperator = 0
  kReadRegister = 1
  kReadLength = 2
  kSendOperator = 3
  kInterrputOperator = 4
  kReadRegisterValue = 5
)

var (
  MOV = []byte{0x01}
  INT = []byte{0x06}
)

var (
  MSA = []byte{0x01}
  RSA = []byte{0x02}
  ASA = []byte{0x03}
)

var (
  RegMsa = 0
  RegRsa = 0
  RegAsa = 5
)

var kMove = []byte{0x31}
var kRound = []byte{0x32}

func IsActiveOperatorHeader(c byte) bool {
  if bytes.Equal([]byte{c}, MOV) || bytes.Equal([]byte{c}, INT) {
    return true
  } else {
    return false
  }
}

func IsActiveRegisterType(c byte) bool {
  if bytes.Equal([]byte{c}, MSA) || bytes.Equal([]byte{c}, RSA) || bytes.Equal([]byte{c}, ASA) {
    return true
  } else {
    return false
  }
}

func ExecutableUsbAvailable() (Errors) {
  f, e := os.Open("program.e")
  
  defer f.Close()

  if e != nil {
    return EXCEPT_FILE_NOT_FOUND
  }

  return 0
}

func main() {
  fmt.Println("\nstart")

  switch ExecutableUsbAvailable() {
  case(EXCEPT_FILE_NOT_FOUND):
    fmt.Fprintln(os.Stderr, "file not found")
    os.Exit(1)
  }

  fmt.Println("file found")

  readFileObject, _ := os.Open("program.e")

  contents := make([]byte, 1024)
  var outputRegister []byte
  var length int
  var outputValue int

  l := 0
  i := 0

  progress := kReadOperator

  _, eDecl := readFileObject.Read(contents)
    if eDecl != nil {
    os.Exit(1)
  }

  for {
    
    switch progress {
    case kInterrputOperator:
      if bytes.Equal([]byte{contents[i]}, kMove) {
        p := exec.Command("python3 ./move_process.py", strconv.Itoa(RegMsa))
        p.Start()
        p.Wait()
      }
      if bytes.Equal([]byte{contents[i]}, kRound) {
        p := exec.Command("python3 ./direction_process.py", strconv.Itoa(RegRsa))
        p.Start()
        p.Wait()
      }

      progress = kReadOperator
    
    case kReadOperator:
      if contents[i] == byte(0x00) {
        fmt.Println("done")
        os.Exit(0)
      }
      if !IsActiveOperatorHeader(contents[i]) {
        fmt.Fprintln(os.Stderr, "binary error: unknown operator")
        break
      }
      
      if bytes.Equal([]byte{contents[i]}, MOV) {
        progress = kReadRegister
      }
      if bytes.Equal([]byte{contents[i]}, INT) {
        progress = kInterrputOperator
      }
    
    case kReadRegister:
      if !IsActiveRegisterType(contents[i]) {
        fmt.Fprintln(os.Stderr, "binary error")
        break
      }
      
      outputRegister = []byte{contents[i]}
      progress = kReadLength

    case kReadLength:
      switch {
      case contents[i] == byte(0x01):
        length = 1

      case contents[i] == byte(0x02):
        length = 2

      case contents[i] == byte(0x03):
        length = 3
      
      case contents[i] == byte(0x04):
        length = 4

      case contents[i] == byte(0x05):
        length = 5

      case contents[i] == byte(0x06):
        length = 6

      case contents[i] == byte(0x07):
        length = 7

      case contents[i] == byte(0x08):
        length = 8

      case contents[i] == byte(0x09):
        length = 9

      default:
        fmt.Fprintln(os.Stderr, "length error", contents[i])
      }

      progress = kReadRegisterValue
      
    
    case kReadRegisterValue:
      if l == length {
        l = -1
        switch {
        case bytes.Equal(outputRegister, MSA):
          fmt.Println("move second value: ", outputValue)
          RegMsa = outputValue
        
        case bytes.Equal(outputRegister, RSA):
          RegRsa = outputValue

        case bytes.Equal(outputRegister, ASA):
          fmt.Println("speed value: ", outputValue)
          RegAsa = outputValue
        }
        progress = kReadOperator
        i--
      }

      d, _ := strconv.Atoi(string([]byte{contents[i]}))

      outputValue, _ = strconv.Atoi(fmt.Sprintf("%d%d", outputValue, d))

      if l == -1 {
        outputValue = 0
      }

      l++
    }
    i++
  }
}
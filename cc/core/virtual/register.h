// Copyright (c) 2021, CuprumProject author.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#ifndef _REGISTER_H_
#define _REGISTER_H_

#include <stddef.h>

typedef short KNOWN_REGISTER_TYPE;

enum VirtualRegister {
  /* variable area */
  rvf = 0,
  rvs = 1,
  rvt = 2,
  /* reference area */
  msa = 3,
  rsa = 4,
  asa = 5,
  /* system reserved area*/
  rvr = 6
};

constexpr const char* kMsa = "msa";
constexpr const char* kRsa = "rsa";
constexpr const char* kAsa = "asa";

KNOWN_REGISTER_TYPE RVF = 0;
KNOWN_REGISTER_TYPE RVS = 0;
KNOWN_REGISTER_TYPE RVT = 0;

KNOWN_REGISTER_TYPE MSA = 0;
KNOWN_REGISTER_TYPE RSA = 0;

KNOWN_REGISTER_TYPE RVR = 0;

#endif
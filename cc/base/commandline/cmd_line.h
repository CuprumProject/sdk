// Copyright (c) 2021, CuprumProject author.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#ifndef _CMD_LINE_H_
#define _CMD_LINE_H_

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <memory>
#include <vector>

namespace CommandLine {

inline bool HasOption(int argCount, char** argValue, const char* index) {
  for (int i = 0; i <= argCount - 1; i++) {
    if (!abs(::strcmp(argValue[i], index)))
      return 1;
  }
  return 0;
}

inline const char* GetValueFromOption(int argCount, char** argValue, const char* index) {
  for (int i = 0; i <= argCount - 1; i++)
    if (!abs(::strcmp(argValue[i], index)))
        return argValue[i + 1];
  
  return 0;
}

}

#endif
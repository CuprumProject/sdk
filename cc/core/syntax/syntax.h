// Copyright (c) 2021, CuprumProject author.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#ifndef _SYNTAX_H_
#define _SYNTAX_H_

#include <string.h>

#include <string>

namespace Reserved {

constexpr const char* kMov = "mov";
constexpr const char* kAdd = "add";
constexpr const char* kSub = "sub";
constexpr const char* kMul = "mul";
constexpr const char* kDiv = "div";
constexpr const char* kInt = "int";

}

[[noreturn]]
void ErrorExit(const char* reason) {
  fprintf(stderr, reason);
  exit(1);
}

inline bool IsSystemReservedSyntaxType(const char* optValue) {
  if (!optValue)
    return 0;
  
  std::string opt = optValue;
  std::string op = opt.substr(0, 3);

  if (op == Reserved::kMov || op == Reserved::kAdd ||
      op == Reserved::kSub || op == Reserved::kMul ||
      op == Reserved::kDiv || op == Reserved::kInt)
    return 1;
  else
    return 0;
}

inline int GetOperator(const char* optValue) {
  std::string opt = optValue;
  std::string op = opt.substr(0, 3);

  if (!abs(op.compare(Reserved::kMov)))
    return 1;
  if (!abs(op.compare(Reserved::kAdd)))
    return 2;
  if (!abs(op.compare(Reserved::kSub)))
    return 3;  
  if (!abs(op.compare(Reserved::kMul)))
    return 4;
  if (!abs(op.compare(Reserved::kDiv)))
    return 5;
  if (!abs(op.compare(Reserved::kInt)))
    return 6;
  return 0;
}

inline std::string GetOperatorToString(const char* code) {
  int i = 0;

  char op[1024] = {0};

  while (code[i] != ' ') {
    sprintf(op, "%s%c", op, code[i]); 
    i++;
    if (i >= 10)
      ErrorExit("Error: instruction expected or parameter too long");
  }

  return op;
}

inline std::string TookFirstArgument(const char* code) {
  int i = 0;

  char firstArgument[1024] = {0};
  char modifiedCode[1024] = {0};
  
  ::strcat(modifiedCode, code);

  while(modifiedCode[i] != ' ')
    i++;
  while(modifiedCode[i] == ' ')
    i++;
  while(!(modifiedCode[i] == ' ' || modifiedCode[i] == '\0' || modifiedCode[i] == '\r' || modifiedCode[i] == '\n' || static_cast<unsigned int>(i) >= strlen(modifiedCode))) {
    sprintf(firstArgument, "%s%c", firstArgument, modifiedCode[i]); 
    i++;
    if (i >= 14)
      ErrorExit("Error: instruction expected or parameter too long");
  }

  return firstArgument;
}

inline int GetFirstArgumentLength(const char* code) {
  return TookFirstArgument(code).length();
}

inline std::string TookSecondArgument(const char* code) {
  int i = 0;

  char secondArgument[1024] = {0};
  char modifiedCode[1024];

  memset(modifiedCode, 0, 1024);

  ::strcat(modifiedCode, code);

  for (int l = 0; l <= 1; l++) {
    while(modifiedCode[i] != ' ')
      i++;
    while(modifiedCode[i] == ' ')
      i++;
    if (l == 1)
      while(!(modifiedCode[i] == '\0' || modifiedCode[i] == ' ' || modifiedCode[i] == '\r' || modifiedCode[i] == '\n' || static_cast<unsigned int>(i) >= strlen(modifiedCode))) {
        sprintf(secondArgument, "%s%c", secondArgument, modifiedCode[i]); 
        i++; 
        if (i >= 14)
          ErrorExit("Error: instruction expected or parameter too long");
      }
  }

  return secondArgument;
}

inline int GetSecondArgumentLength(const char* code) {
  return TookSecondArgument(code).length();
}

#endif // _SYNTAX_H_
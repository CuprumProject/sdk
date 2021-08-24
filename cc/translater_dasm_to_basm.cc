// Copyright (c) 2021, CuprumProject author.
// All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#include <string>
#include <sstream>
#include <memory>

#include "core/syntax/syntax.h"
#include "core/virtual/register.h"

#include "base/commandline/cmd_line.h"
#include "base/file/file_access.h"

using CommandLine::HasOption;
using CommandLine::GetValueFromOption;

using fsys::CreateSingleFileTrace;
using fsys::File;

int DetermineTargetAndCompile(const char*);

int main(int argc, char* argv[]) {
  std::unique_ptr<const char*> compileObject = (
    HasOption(argc, argv, "-t") ? 
      std::make_unique<const char*>(GetValueFromOption(argc, argv, "-t")) :
      nullptr
  );

  if (compileObject == nullptr || !*compileObject) {
    fprintf(stderr, "An error occurred loading file\n");
    if (compileObject == nullptr)
      fprintf(stderr, "Hint: Use -t option to set the file to compile.");

    return 1;
  }

  return DetermineTargetAndCompile(*compileObject);
}

int DetermineTargetAndCompile(const char* compileObjectFileName) {
  File compileObjectFile = CreateSingleFileTrace(compileObjectFileName);
  File textout = CreateSingleFileTrace("t.da");
  
  const std::string contents = reinterpret_cast<char*>(*(compileObjectFile.Read()));

  std::string line;
  std::string write;
  std::string f;
  std::string op;

  std::stringstream StringStream(contents);

  while (std::getline(StringStream, line, '\n')) {
    op = GetOperatorToString(line.c_str());
    printf(op.c_str());

    if (op == "speed") {
      f = TookFirstArgument(line.c_str());
      write += "mov asa " + f;
      write += "\n";

    } else if (op == "move") {
      f = TookFirstArgument(line.c_str());
      write += "mov msa " + f;
      write += "\n";
      write += "int 1";
      write += "\n";

    } else if (op == "direction") {
      f = TookFirstArgument(line.c_str());
      write += "mov rsa " + f;
      write += "\n";
      write += "int 2";
      write += "\n";

    } else {
      fprintf(stderr, "error: Unknown operator");
      exit(1);
    }
  }
  
  printf(write.c_str());

  textout.Write(write.c_str());

  printf("\ntranslate success");
  return 0;
}